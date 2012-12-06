#include "ModelLoader.h"


//might need this for new versions of the SDK
//#include <fbxsdk/utils/>

CModelLoader::CModelLoader()
{
}

CModelLoader::~CModelLoader()
{
}

CMeshComponent * CModelLoader::createCube(ID3D10Device *pDevice,float width, float height, float length)
{
	CMeshComponent * pMesh=new CMeshComponent();

    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3( -width/2.0f, height/2.0f, length/2.0f ),D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.0f)}, //0 top left 
        {D3DXVECTOR3( width/2.0f, -height/2.0f, length/2.0f ),D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,1.0f)},//1 bottom right 
        {D3DXVECTOR3( -width/2.0f, -height/2.0f, length/2.0f ),D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,1.0f)}, //2 bottom left
		{D3DXVECTOR3( width/2.0f, height/2.0f, length/2.0f ), D3DXCOLOR(0.0f,1.0f,0.0f,1.0f), D3DXVECTOR2(1.0f,0.0f)}, //3 top right

		//back
        {D3DXVECTOR3(-width/2.0f, height/2.0f, -length/2.0f ),D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),D3DXVECTOR2(0.0f,0.0f)}, //4 top left
        {D3DXVECTOR3( width/2.0f, -height/2.0f, -length/2.0f ),D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,1.0f)},//5 bottom right
        {D3DXVECTOR3(-width/2.0f, -height/2.0f, -length/2.0f), D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,1.0f)}, //6 bottom left
		{D3DXVECTOR3( width/2.0f, height/2.0f, -length/2.0f),D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,0.0f)} //7 top right
    };

	int indices[]={0,1,2,0,3,1,//front
					4,5,6,4,7,5, //back
					0,6,4,0,2,6, //left
					3,5,7,3,1,5, //right
					0,4,3,0,3,7, //top
					2,6,1,6,5,1	 //bottom
					};

	CGeometry * pGeom=new CGeometry(pDevice);
	for (int i=0;i<8;i++)
	{
		pGeom->addVertex(vertices[i]);
	}

	for (int i=0;i<36;i++)
	{
		pGeom->addIndex(indices[i]);
	}
	pMesh->addSubset(pGeom);

	return pMesh;
}

CMeshComponent *CModelLoader::loadModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CMeshComponent * pRenderable=NULL;
	string extension=filename.substr(filename.find('.')+1);

	if (extension.compare("fbx")==0)
		pRenderable=loadFbxModelFromFile(pDevice,filename);

	return pRenderable;
}

bool CModelLoader::loadTextureCoords(const FbxMesh *pMesh, Vertex *pVerts, int noVerts)
{
	//extract texture coordinates
   //get all UV set names
   FbxStringList lUVSetNameList;
   pMesh->GetUVSetNames(lUVSetNameList);
   //iterating over all uv sets
   for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
   {
        //get lUVSetIndex-th uv set
        const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
        if(!lUVElement)
            continue;

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if( lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint )
            return false;

        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int lIndexCount= (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

        //iterating through the data by polygon
        const int lPolyCount = pMesh->GetPolygonCount();
        if( lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint )
        {
            for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    FbxVector2 lUVValue;

                    //get the index of the current vertex in control points array
                    int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex,lVertIndex);

                    //the UV index depends on the reference mode
                    int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
					pVerts[lPolyVertIndex].TexCoords.x=lUVValue[0];
					pVerts[lPolyVertIndex].TexCoords.y=1-lUVValue[1];
                }
            }
        }
       else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            int lPolyIndexCounter = 0;
            for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    if (lPolyIndexCounter < lIndexCount)
                    {
                        FbxVector2 lUVValue;

                        //the UV index depends on the reference mode
                        int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						
                        //User TODO:
                        //Print out the value of UV(lUVValue) or log it to a file
						//Something in here, verts don't refer correctly
						int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex,lVertIndex);
						pVerts[lPolyVertIndex].TexCoords.x=lUVValue[0];
						pVerts[lPolyVertIndex].TexCoords.y=1-lUVValue[1];
                        lPolyIndexCounter++;
                    }
                }
			}
		}
   }

   return true;
}


bool CModelLoader::loadFromMesh(const FbxMesh *pMesh, CMeshComponent * pMeshComponent)
{
   //Vertices
   FbxVector4* lControlPoints = pMesh->GetControlPoints();
   int numberOfVerts=pMesh->GetControlPointsCount();
   
   //Indices
   int noIndices=pMesh->GetPolygonVertexCount();
   int *pIndices=pMesh->GetPolygonVertices();

   Vertex * pVerts=new Vertex[numberOfVerts];
   for(int i=0;i<numberOfVerts;i++)
   {
	   pVerts[i].Pos.x=lControlPoints[i][0];
	   pVerts[i].Pos.y=lControlPoints[i][1];
	   pVerts[i].Pos.z=lControlPoints[i][2];
   }
   
   loadTextureCoords(pMesh,pVerts,numberOfVerts);
   
   
   CGeometry * pRenderable=new CGeometry(m_pD3DDevice);
   for (int i=0;i<numberOfVerts;i++)
   {
	   pRenderable->addVertex(pVerts[i]);
   }
   for (int i=0;i<noIndices;i++)
   {
	   pRenderable->addIndex(pIndices[i]);
	}

   if (pVerts)
   {
	   delete[] pVerts;
	   pVerts=NULL;
   }

   pMeshComponent->addSubset(pRenderable);

   return true;
}

CMeshComponent * CModelLoader::loadFbxModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CMeshComponent * pMeshComponent=new CMeshComponent();
	m_pD3DDevice=pDevice;

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
	FbxGeometryConverter converter( lSdkManager);
	

    // Use the first argument as the filename for the importer.
	if(!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
		return NULL;
    }

	// Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");
	FbxAxisSystem SceneAxisSystem = lScene->GetGlobalSettings().GetAxisSystem();
    //FbxAxisSystem::DirectX.ConvertScene( lScene );



    INT iUpAxisSign;
	FbxAxisSystem::EUpVector UpVector = SceneAxisSystem.GetUpVector( iUpAxisSign );

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);
	converter.SplitMeshesPerMaterial(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

	FbxNode* lRootNode = lScene->GetRootNode();
	FbxMesh * pMesh=NULL;
	if(lRootNode) {
		for (int i=0;i<lRootNode->GetChildCount();i++){
			FbxNode * modelNode=lRootNode->GetChild(i);
			for(int i=0;i<modelNode->GetNodeAttributeCount();i++)
			{
				FbxNodeAttribute *pAttributeNode=modelNode->GetNodeAttributeByIndex(i);
				if (pAttributeNode->GetAttributeType()==FbxNodeAttribute::eMesh)
				{
					//found mesh
					pMesh=(FbxMesh*)pAttributeNode;
					if (pMesh)
					{
						pMesh=converter.TriangulateMesh(pMesh);
						loadFromMesh(pMesh,pMeshComponent);
					}
				}
			}
		}
	}

	lSdkManager->Destroy();


	

	return pMeshComponent;
}


void CModelLoader::computeTangents(Vertex *pVerts,int vertexCount)
{
	int triCount=vertexCount/3;
	D3DXVECTOR3 * tan1=new D3DXVECTOR3[vertexCount];
	D3DXVECTOR3 * tan2=new D3DXVECTOR3[vertexCount];

	for (int i=0;i<triCount;i+=3)
	{
		D3DXVECTOR3 v1=pVerts[i].Pos;
		D3DXVECTOR3 v2=pVerts[i+1].Pos;
		D3DXVECTOR3 v3=pVerts[i+2].Pos;

		D3DXVECTOR2 uv1=pVerts[i].TexCoords;
		D3DXVECTOR2 uv2=pVerts[i+1].TexCoords;
		D3DXVECTOR2 uv3=pVerts[i+2].TexCoords;
			
		float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = uv2.x - uv1.x;
        float s2 = uv3.x - uv1.x;
        float t1 = uv2.y - uv1.y;
        float t2 = uv3.y - uv1.y;

		float r=1.0f/(s1*t2-s2*t1);
		D3DXVECTOR3 sdir = D3DXVECTOR3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
        D3DXVECTOR3 tdir = D3DXVECTOR3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

        tan1[i] += sdir;
        tan1[i+1] += sdir;
        tan1[i+2] += sdir;

        tan2[i] += tdir;
        tan2[i+1] += tdir;
        tan2[i+2] += tdir;
	}
	for (int i=0;i<vertexCount;i++)
	{
		D3DXVECTOR3 n=pVerts[i].Normal;
		D3DXVECTOR3 t=tan1[i];
		D3DXVECTOR3 tmp = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&tmp,&tmp);
		pVerts[i].Tangent = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
        //tangents[a].w = (Vector3.Dot(Vector3.Cross(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;

	}
	if (tan1)
	{
		delete [] tan1;
		tan1=NULL;
	}
	if (tan2)
	{
		delete [] tan2;
		tan2=NULL;
	}
}