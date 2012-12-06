//Global Variables 
float4x4 matWorld:WORLD<string UIWidget="None";>;
float4x4 matView:VIEW<string UIWidget="None";>;
float4x4 matProjection:PROJECTION<string UIWidget="None";>;

//Creates a float 4 with a string with UIName AmbientMaterial and a UIWidget with a colour wheel to change colour.
float4 ambientMaterialColour<
	string UIName="Ambient Material";
	string UIWidget="Color";
> =float4(0.5f,0.5f,0.5f,1.0f);

//Creates a float 4 with a string with UIName DiffuseMaterial and a UIWidget with a colour wheel to change colour.
float4 diffuseMaterialColour<
	string UIName="Diffuse Material";
	string UIWidget="Color";
> =float4(0.8f,0.8f,0.8f,1.0f);

//Creates a float 4 with a string with UIName SpecularMaterial and a UIWidget with a colour wheel to change colour.
float4 specularMaterialColour<
	string UIName="Specular Material";
	string UIWidget="Color";
> =float4(1.0f,1.0f,1.0f,1.0f);

//Creates a float 4 with a string with UIName BumpMaterial and a UIWidget with a slider to change values.
float4 bumpMaterialColour<
	string UIName="Bump Material";
	string UIWidget="Slider";
	float UIMin = 0.0;
	float UIMax = 100.0;
	float UIStep = 5.0;
	> =25.0f;

//Creates a float 4 with a string with UIName HeightMaterial and a UIWidget with a slider to change values.
float4 heightMaterialColour<
	string UIName="Height Material";
    string UIWidget="Slider";
	float UIMin = 0.0;
	float UIMax = 100.0;
	float UIStep = 5.0;
	> =25.0f;
	
float4 ambientLightColour:AMBIENT; //Creates a float 4 called ambientLightColour with reference to the semantic AMBIENT.


float4 lightDirection:DIRECTION< //Creates a float 4 called lightDirection with reference to the semantic DIRECTION.
	
>;

float4 diffuseLightColour:DIFFUSE< //Creates a float 4 called diffuseLightColour with reference to the semantic DIFFUSE.
>;

float4 specularLightColour:SPECULAR< //Creates a float 4 called specularLightColour with reference to the semantic SPECULAR.
>;

float specularPower< 
	string UIName="Specular Power";
	string UIWidget="Slider"; //Creates a float called SpecularPower which allows the value to be changed using a slider.
	float UIMin = 0.0;
	float UIMax = 100.0; //This sets the values for the slider.
	float UIStep = 5.0;
> =25.0f;


float reflectionPower<
	string UIName="Reflection Power";  //Creates a float called ReflectionPower which allows the value to be changed using a slider.
	string UIWidget="Slider";
	float UIMin = 0.0;
	float UIMax = 100.0;  //This sets the values for the slider.
	float UIStep = 3.0;
> =25.0f;


float4 cameraPosition:POSITION<  //Creates a float 4 called cameraPositon with rederence to the semantic POSITION.
	string Object ="Perspective";
>;

Texture2D diffuseMap; //Declares a 2D texture called diffuseMap.
bool useDiffuseTexture
<
string UIName="Use Diffuse Texture"; // Allows the diffuse texture to be set to true or false.
> = false;

Texture2D specularMap;//Declares a 2D texture called specularMap
bool useSpecularTexture
<
string UIName="Use Specular Texture"; //Allows the specular texture to be set to true or false.
> = false;


Texture2D bumpMap;//Declares a 2D texture called bumpMap
bool useBumpTexture
<
string UIName="Use Bump Texture"; //Allows the bump texture to be set to true or false.
> = false;


Texture2D heightMap;//Declares a 2D texture called heightMap
bool useheightTexture
<
string UIName="Use height Texture"; //Allows the height texture to be set to true or false.
> = false;


TextureCube envMap; //Creates a texture cube called envMap.

float reflectionpower=0.0f; //sets the reflection power to 0.

SamplerState wrapSampler //Creates a SamplerState called wrapSampler.
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = CLAMP; // These are the vaues that are used for the samplerstate.
    AddressV = CLAMP;
};

struct VS_INPUT //a struct setting the values that can be set in the vertex shader input function.
{
	float4 pos:POSITION;
	float4 colour:COLOR;
	float3 normal:NORMAL0;
	float2 texCoord:TEXCOORD0;
};

struct PS_INPUT //a struct setting the values that can be set in the pixel shader input function.
{
	float4 pos:SV_POSITION;
	float4 colour:COLOR;
	float3 cameraDirection:VIEWDIR;
	float2 texCoord:TEXCOORD0;
	float3 normal:NORMAL;
	float3 lightDir:LIGHTDIR;
	float3 reflectTexCoord:TEXCOORD1;
};

PS_INPUT VS(VS_INPUT input) //The input for the vertex shader.
{
	PS_INPUT output=(PS_INPUT)0;
	
	float4x4 matViewProjection=mul(matView,matProjection);
	float4x4 matWorldViewProjection=mul(matWorld,matViewProjection); // These are the matrix caculations.
	float4 worldPos=mul(input.pos,matWorld);
	output.normal=normalize(mul(input.normal,matWorld));
	output.cameraDirection=mul(normalize(cameraPosition-worldPos),matWorld);//These are the calculations for the output.
	output.lightDir=lightDirection;			
	
	
	output.pos=mul(input.pos,matWorldViewProjection);

	output.texCoord=input.texCoord;

	output.reflectTexCoord=reflect(cameraPosition-worldPos,input.normal);
	return output; //returns the output values.
}




float4 PS(PS_INPUT input):SV_TARGET //The input for the pixel shader.
{
	float3 normal=input.normal;	
	float3 lightDir=normalize(input.lightDir);
	
	float4 diffuseColour=diffuseMaterialColour;
	float4 specularColour=specularMaterialColour;
	float4 bumpColour=bumpMaterialColour;
	float4 heightColour=heightMaterialColour;
	
	if (useDiffuseTexture)
		diffuseColour=diffuseMap.Sample(wrapSampler,input.texCoord);
	if (useSpecularTexture)
		specularColour=specularMap.Sample(wrapSampler,input.texCoord); //If functions for the textures. If their values are set to true these calculations are carried out.
	if (useBumpTexture)
		bumpColour=bumpMap.Sample(wrapSampler,input.texCoord);
	if (useheightTexture)
		heightColour=heightMap.Sample(wrapSampler,input.texCoord);
		
		
	float diffuse=saturate(dot(normal,lightDir));
	
	float3 halfVec=normalize(lightDir+input.cameraDirection); 
	float specular=pow(saturate(dot(normal,halfVec)),specularPower); //Dot product calculation.
	
	float4 reflectionColour = envMap.Sample(wrapSampler,input.reflectTexCoord)*reflectionPower; //reflection colour calculation using the envMap value.
	
	return ((ambientMaterialColour*ambientLightColour)+
	(diffuseColour*diffuseLightColour*diffuse)+
	(specularColour*specularLightColour*specular))+reflectionColour; //This returns the following calculation.
}

RasterizerState DisableCulling //Creates a RasterizerState called DisableCulling.
{
    CullMode = NONE;//CullMode value is set to none.
};

DepthStencilState EnableZBuffering//Creates a DepthStencilState called EnableZBuffering
{
	DepthEnable =TRUE;//DepthEnable value is set to true.
};

technique10 Render //This is the code for the render pass.
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0,  PS() ) ); //The values that are set when it renders.
		SetRasterizerState(DisableCulling); 
		SetDepthStencilState(EnableZBuffering,0);
	}
}