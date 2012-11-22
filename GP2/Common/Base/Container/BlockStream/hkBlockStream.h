/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2012 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
//HK_REFLECTION_PARSER_EXCLUDE_FILE

#ifndef HK_BLOCKSTREAM_H
#define HK_BLOCKSTREAM_H


class hkBlockStreamAllocator;
class hkThreadLocalBlockStreamAllocator;
class hkBlockStreamWriterBase;


struct hkBlockStreamBase
{

	class Consumer;
	class Reader;
	class Range;
	class Writer;
	class Stream;

	/// The basic storage container for the hkBlockStream::Stream. A stream consists of a consecutive number of blocks.
	/// Each block can be filled with values/objects of a shared size (maximum allowed size is BLOCK_DATA_SIZE).
	// Block should be aligned to 'BLOCK_ALIGNMENT' bytes.
	struct Block
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Block );

		enum
		{
		#if defined( HK_TINY_SPU_ELF )
			BLOCK_TOTAL_SIZE = 512, 
		#elif defined(HK_PLATFORM_HAS_SPU)
			BLOCK_TOTAL_SIZE = 1024*2, 
		#elif defined(HK_PLATFORM_CTR)
			BLOCK_TOTAL_SIZE = 512, 
		#else
			BLOCK_TOTAL_SIZE = 1024*4, 
		#endif
			BLOCK_HEADER_SIZE = 32,
			BLOCK_DATA_SIZE = BLOCK_TOTAL_SIZE - BLOCK_HEADER_SIZE,
			BLOCK_ALIGNMENT = 128,
		};

		typedef hkUint16 CountType;

		HK_FORCE_INLINE void setHeaderToZero();

		Block()
		{
			setHeaderToZero();
		}

		void*		begin()			{ return m_data; }
		const void* begin() const   { return m_data; }

		void*		end()		{ return this+1; }
		const void* end() const { return this+1; }

		
		/// The total number of elements in this block which have not been consumed yet.
		HK_ALIGN16( hkUint32 m_numElements );

		CountType	m_bytesUsed;				///< The number of bytes used in this block 
		int			m_blockIndexInStream;		///< The index of this block in a stream
		Block* m_next;							///< Next block in this linked list
		hkUint32 /*hkBlockStreamAllocator**/ m_allocator;	///< for debugging check, storing the last 32 bit of the pointer
		hkUint32 /*Stream**/				 m_debugBlockStream;		///< for debugging, storing the last 32 bit of the pointer
		HK_ALIGN_REAL( hkUchar m_data[ BLOCK_DATA_SIZE ] );	///< the data
	};

	/// A range in a stream.
	/// Ranges are pointing to blocks not blockStreams, so a range will still
	/// be valid if a blockstream gets appended to another one
	class Range
	{
		public:

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Range );

			HK_FORCE_INLINE	Range()
			:	m_numElements(0)
			{
			}

			HK_FORCE_INLINE	void clearRange()
			{
				m_startBlock  = HK_NULL;
				m_numElements = 0;
			}

			HK_FORCE_INLINE bool isEmpty() const { return m_numElements == 0; }

			HK_FORCE_INLINE int  getNumElements() const { return m_numElements; }

			//HK_FORCE_INLINE void setNumElements(int num) { m_numElements = num; }

			HK_FORCE_INLINE void setStartPoint( const hkBlockStreamBase::Writer* HK_RESTRICT it );

			void setEndPoint( const Writer* HK_RESTRICT it );

			// Use setStartPointExclusive and  setEndPointExclusive to create exclusive ranges 
			// A range is exclusive if it doesn't share any blocks with other ranges.

			HK_FORCE_INLINE void setStartPointExclusive( hkBlockStreamBase::Writer* HK_RESTRICT it );

			HK_FORCE_INLINE	void setEndPointExclusive( Writer* HK_RESTRICT it );


			HK_FORCE_INLINE void setEmpty() { clearRange(); }

			HK_ON_CPU( HK_FORCE_INLINE void setEntireStream( hkBlockStreamBase::Stream* HK_RESTRICT stream ) );

	public:

			/// A pointer to the block that holds the first element of THIS range.
			HK_ALIGN16(Block* m_startBlock);

			/// The offset of the first element inside the start block (m_startBlock).
			Block::CountType m_startByteLocation;

			/// The number of elements in the first block. 
			/// Note: when setStartPoint() is called this is the start element index in the start block.
			Block::CountType m_startBlockNumElements;

			/// The length of THIS range (i.e. the number of elements in THIS range).
			/// Note: this value is actually negative!
			int m_numElements;
	};

	class LinkedRange : public Range
	{
		public:

			HK_FORCE_INLINE	LinkedRange()
			:	Range()
			,	m_next(HK_NULL)
			{
			}

			/// Append another range to this. The range MUST be persistent in memory as it will be referenced by this.
			HK_ON_CPU( template<typename T> HK_FORCE_INLINE void appendPersistentRange( T* HK_RESTRICT range ); );


			/// Append another range to this. The range MUST be persistent in memory as it will be referenced by this.
			/// rangeSpu is the range that will be appended, and rangePpu is the persistent address to which 
			/// the appended range (rangeSpu) should be DMA'd by the caller before or after calling this function.
			HK_ON_SPU( template<typename T> HK_FORCE_INLINE void appendPersistentRangeSpu( T* HK_RESTRICT rangeSPU, T* HK_RESTRICT rangePpu); );

			HK_FORCE_INLINE	void clearRange()
			{
				Range::clearRange();
				m_next = HK_NULL;
			}

		public:

			/// A pointer to the next range if several ranges have been linked.
			/// Note: that next range is completely independent of THIS range, i.e. m_numElements does not cross
			/// any range boundaries.
			LinkedRange*	m_next;	
	};

	///	when a block is allocated, it keeps the ptr in this class. 
	///	when hkBlockStream is destroyed, all the blocks owned by this stream are freed.
	class Stream
	{
		public:
			/// The allocator used by this stream.
			typedef hkThreadLocalBlockStreamAllocator Allocator;

			friend class hkBlockStreamBase::Consumer;
			friend class hkBlockStreamBase::Reader;
			friend class hkBlockStreamBase::Range;
			friend class hkBlockStreamBase::Writer;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Stream );

			/// Convenience constructor.
			HK_FORCE_INLINE Stream( Allocator* tlAllocator, bool zeroNewBlocks = false ){ initBlockStream( tlAllocator, zeroNewBlocks );	}

			// empty constructor, call initBlockStream to initialize
			HK_FORCE_INLINE Stream(){}

			/// Destructor. You have to call clear() before calling the destructor (since destructors do not take parameters).
			HK_FORCE_INLINE ~Stream();

			void initBlockStream( Allocator* tlAllocator, bool zeroNewBlocks = false );

			///	Resets the stream. After calling reset, block stream has 1 empty block.
			void reset( Allocator* HK_RESTRICT tlAllocator );

			/// append blockstream to this stream and delete inStream
			void append( Allocator* HK_RESTRICT tlAllocator, Stream* inStream );

			HK_FORCE_INLINE bool isEmpty() const;

			HK_FORCE_INLINE int getTotalNumElems() const;

			/// Checks the consistency of the block headers, not the block contents
			void checkConsistency( ) const;

			/// Checks whether an input array of ranges is consistent with the block stream
			void checkConsistencyWithGrid( const Range* rangesIn, int numRanges, int rangeStriding, bool allowForUnusedData ) const;

			void checkConsistencyOfRange( const Range& range );


			/// Call this if you:
			///   - Want to reuse this blockstream after you consumed some blocks.
			///		See Consumer.
			void fixupConsumedBlocks(Allocator* HK_RESTRICT tlAllocator);

			// Call to release all blocks that precede the given range
			void freeAllBlocksBeforeRange(Allocator* HK_RESTRICT tlAllocator, const Range *range);

			//	clears the stream, does not reset it, only use if you call append immediately after this
			void clear( Allocator* HK_RESTRICT tlAllocator );

			// returns the total number blocks * sizeof(Block)
			int getTotalBytesAllocated() const;

			/// Move an exclusive range to the back of the stream. A range is exclusive if it doesn't share
			/// any blocks with other ranges.
			void moveExclusiveRangeToBack( Allocator* HK_RESTRICT tlAllocator, const Range* rangeInOut );

		protected:

			/// append a list of ranges, creates lots of holes but is really fast
			void append( Allocator* HK_RESTRICT tlAllocator, Range* ranges, int numRanges, int rangeStriding );

			HK_ON_CPU( HK_FORCE_INLINE const Block* begin() const { return m_blocks[0]; } )
			HK_ON_SPU( const Block* begin() const; )

			/// return the first block
			HK_FORCE_INLINE Block* beginRw(){ return const_cast<Block*>(begin()); }

			Block* last()
			{	return m_blocks[ m_blocks.getSize()-1 ]; }
			;

			const Block* last() const { return const_cast<Stream*>(this)->last(); }

			/// allocate and append a new block
			Block* blockAlloc(Allocator* tlAllocator);

			/// Remove and free the last block of the stream.
			Block* popBack( Allocator* tlAllocator );

			void freeBlockWithIndex(Allocator* HK_RESTRICT tlAllocator, Block* HK_RESTRICT blockPpu, int index);

			enum { INPLACE_PBLOCK_STORAGE = 24 };

		public:

			protected:		hkBlockStreamAllocator* m_allocator;	
			public: 		hkBool	m_partiallyFreed;	// set if some blocks of the blockstream are freed
			public:			hkBool  m_zeroNewBlocks;			///< if set, all new blocks will be zeroed
			protected:		hkBool	m_isLocked;	// locked by writers and readers
			protected:		hkBool  m_spuWronglySentConsumedBlockStreamBack;	// if this flag is set, a blockstream was sent back to the ppu which was touched by a consumer
							int		m_numTotalElements;	///< The total number of elements in this stream. This value is only valid after a call to Writer::finalizeLastBlock().
		private:

			// Blocks owned by this stream.
			// This array is aligned because it needs to be accessed using Spu dma transfers.
			hkInplaceArrayAligned16< Block*, INPLACE_PBLOCK_STORAGE > m_blocks;
	};



	//
	//	Iterators
	//

	/// Implementation class, use hkBlockStream<TYPE>::Writer
	class Reader
	{
		public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Reader );

			HK_FORCE_INLINE	Reader();
			HK_FORCE_INLINE ~Reader();

			void initSpu( int dmaGroup, int numActiveBlocks, const char* what ){}
			void exitSpu(){}

			template<typename T>
			HK_FORCE_INLINE T* spuToPpu(T* t) const { HK_ASSERT( 0xf03df590, unsigned(hkGetByteOffsetInt(m_currentBlock, t)) < sizeof(Block)); return t; }


			/// Sets it to the start of a stream, assumes stream is an spu pointer
			void setToStartOfStream( const Stream* stream );

			/// Sets it to range, assume range is a ppu pointer
			void setToRange( const Range* range );

			HK_FORCE_INLINE void setEmpty(); 

			HK_FORCE_INLINE int getNumUnreadElementsInThisBlock(){ return m_numElementsToReadInThisBlock; }


		protected:
			/// advance and access the next element, assumes the current entry is valid
			HK_FORCE_INLINE	const void* advanceAndAccessNext( int thisElemSize );

			HK_FORCE_INLINE	const void* access();

		protected:
			const void* advanceToNewBlock( );

		protected:

			friend class Range;
			HK_PAD_ON_SPU(const Block*)	m_currentBlock;
			HK_PAD_ON_SPU(const char*)  m_currentByteLocation;
			HK_PAD_ON_SPU(int) m_numElementsToReadInOtherBlocks;
			HK_PAD_ON_SPU(int) m_numElementsToReadInThisBlock;
	};

	/// Implementation class, use hkBlockStream<TYPE>::Writer
	class Writer
	{
		public:

				friend class Range;


		public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Writer );

				/// Ctor
				HK_FORCE_INLINE Writer();

				/// Destructor
				HK_FORCE_INLINE ~Writer();

				/// Set to the end to an existing stream to append data
				void setToEndOfStream( Stream::Allocator* allocator, Stream* blockStreamPpu);

				void setToStartOfStream( Stream::Allocator* allocator, Stream* blockStreamPpu);

				/// Advance by numBytes. This will move the write location in the current block.
				HK_FORCE_INLINE void advance( int numBytes );

				/// Reserve and advance a number of reservations in one go.
				/// This is faster than calling reserve and advance for large numbers of reservations, however, no addresses are returned for writing.
				void batchReserveAndAdvance( int numReservations, int numBytesPerReservation );

				/// Call this method to put the stream back in a consistent and trimmed state.
				/// On spu this will trigger the (final) write-back to ppu memory.
				void finalize();

				/// Returns the total number of elements in the writer's stream. This method should only be called
				/// after a call to finalize().
				HK_FORCE_INLINE int getTotalNumElems();

			#if defined(HK_DEBUG)
				HK_FORCE_INLINE void checkConsistency()
				{
					if (m_blockStream) { m_blockStream->checkConsistency(); }
				}
			#endif


		protected:

				/// Will make sure that the next reserve will access a new block. It is used to create exclusive ranges.
				HK_FORCE_INLINE void setCurrentBlockFull();


				/// Return the address where we are allowed to write \a numBytes number of bytes to. This method will move
				/// to the next block in the stream if the free storage space in the current block is not enough.
				HK_FORCE_INLINE	void* reserveBytes( int numBytes );

				/// Allocates a new block, appends it to the end of the stream and returns the pointer to the new
				/// block's storage space.
				void *allocateAndAccessNewBlock();

				/// Advance by \a currentNumBytes in the stream and return a pointer to storage space where
				/// \a reservedNumBytes can be written to.
				HK_FORCE_INLINE	void* advanceAndReserveNext( int currentNumBytes, int reservedNumBytes );

				/// Wrap up the current block and link the next block. Make sure that \a thisBlock is actually pointing to
				/// the last block in the stream!
				HK_FORCE_INLINE void finalizeLastBlock( Block* HK_RESTRICT thisBlock, Block* HK_RESTRICT nextBlock, int thisBlockNumElements, int bytesUsed );

		public:

				HK_PAD_ON_SPU(Stream*)				m_blockStream;			///< The block stream this Writer is working on.

		protected:

				HK_PAD_ON_SPU(Block*)				m_currentBlock;			///< The current block data is written to.
				HK_PAD_ON_SPU(int)					m_currentByteLocation;	///< The current byte location where data is written to in the current block (m_currentBlock). This value is an offset into the current block and NOT an absolute address.
				HK_PAD_ON_SPU(int)					m_currentBlockNumElems;	///< The number of elements stored in the current block.
				HK_PAD_ON_SPU(Stream::Allocator*)	m_tlAllocator;

				hkBool								m_finalized;			///< Debugging flag
				int									m_accessSize;			///< Debugging value : the numBytes used in the last call to reserveBytes()

	};

	/// Implementation class, don't use directly, use hkBlockStream<TYPE>::Consumer instead
	class Consumer : public Reader
	{
		public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Consumer );

			HK_FORCE_INLINE	Consumer(){}

			void setToStartOfStream( Stream::Allocator* allocator, Stream* stream, Stream* m_blockStreamPpu );


		protected:

			/// Set a range. Use derived classes to call this function.
			void setToRange( Stream::Allocator* allocator, Stream* stream, Stream* m_blockStreamPpu, const Range* range );

			/// Advance and access the next element, freeing the old data. Assumes the current entry is valid.
			HK_FORCE_INLINE	const void* consumeAndAccessNext( int thisElemSize );

			/// free current block
			const void* freeAndAdvanceToNewBlock( );

			// Non-const casting
			Block* getCurrentBlock()			{ return const_cast<Block*>((const Block*)m_currentBlock); }
			char*  getCurrentByteLocation()		{ return const_cast<char*> ((const char*) m_currentByteLocation); }

		protected:
			friend class Range;
			Stream*	 m_blockStream;	// points to spu version
			HK_PAD_ON_SPU(int)		 m_numElementsToFreeInThisBlock;
			HK_PAD_ON_SPU(Stream::Allocator*) m_allocator;
	};

	class Modifier : public Reader
	{
		public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Modifier );

			HK_FORCE_INLINE	Modifier(){  }

			HK_FORCE_INLINE void setToStartOfStream( Stream::Allocator* allocator, Stream* stream, Stream* m_blockStreamPpu );

			HK_FORCE_INLINE void setToRange( const Range* range );

		protected:

			HK_FORCE_INLINE	void* advanceAndAccessNext( int thisElemSize );

			HK_FORCE_INLINE	void* access(){ return const_cast<void*>( Reader::access() );}

			void* advanceToNewBlock( );

			// Non-const casting
			Block* getCurrentBlock()			{ return const_cast<Block*>( (const Block*)m_currentBlock); }
			char*  getCurrentByteLocation()		{ return const_cast<char*> ( (const char*) m_currentByteLocation); }

		protected:

			HK_PAD_ON_SPU(Stream::Allocator*) m_allocator;
	};

};






/// A virtual memory stream of data.
/// Rules:
///    - Structures/Chunks of different sizes can be put into a blockstream.
///    - A blockstream reader has a peek method, which allows you to access the next block of bytes
///      before consuming them.
///    - The writer just reserves bytes, so you can put data into the reserved area. 
///      Any time later you can advance the stream by up to the reserved number of bytes.
template<typename T>
class hkBlockStream: public hkBlockStreamBase::Stream // > hk???
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockStream );

		/// Ctor
		hkBlockStream( Allocator* tlAllocator, bool zeroNewBlocks = false )
		:	hkBlockStreamBase::Stream( tlAllocator, zeroNewBlocks )
		{
		}

		/// Ctor. You need to manually call initBlockStream() before you can use this class.
		hkBlockStream()
		:	hkBlockStreamBase::Stream()
		{
		}

		/// Destructor
		HK_FORCE_INLINE ~hkBlockStream()
		{
		}

		/// append blockstream to this stream and delete inStream
		void clearAndSteal( Allocator* tlAllocator, hkBlockStream<T>* inStream ){ hkBlockStreamBase::Stream::clear(tlAllocator); hkBlockStreamBase::Stream::append(tlAllocator, inStream); }

		//
		/// Helper class to write to a Block stream.
		//
		class Writer : public hkBlockStreamBase::Writer
		{
			public:

				/// Write \a numBytes of \a data to the stream. \a numBytes has to be a multiple of 16.
				HK_FORCE_INLINE T* write16( const T* HK_RESTRICT data, int numBytes )
				{
					HK_ASSERT(0xaf142e11, (numBytes & 0x0f) == 0);
					T *dstCache = (T*)hkBlockStreamBase::Writer::reserveBytes( numBytes );
					hkString::memCpy16NonEmpty( dstCache, data, numBytes>>4 );
					advance( numBytes );
					return dstCache;
				}

				template <typename T2>
				HK_FORCE_INLINE T2* write16( const T2* HK_RESTRICT data, int numBytes )
				{
					HK_ASSERT(0xaf142e11, (numBytes & 0x0f) == 0);
					T2 *dstCache = (T2*)hkBlockStreamBase::Writer::reserveBytes( numBytes );
					hkString::memCpy16NonEmpty( dstCache, data, numBytes>>4 );
					advance( numBytes );
					return dstCache;
				}

				/// Write \a numBytes of \a data to the stream.
				/// If it is, use write16() instead.
				HK_FORCE_INLINE T* write( const T* HK_RESTRICT data, int numBytes )
				{
					T *dstCache = (T*)hkBlockStreamBase::Writer::reserveBytes( numBytes );
					hkString::memCpy4( dstCache, data, numBytes>>2 );
					advance( numBytes );
					return dstCache;
				}

				/// Write \a data to a stream if \a data has getSizeInBytes() implemented and its size is a multiple
				/// of 16.
				HK_FORCE_INLINE T* write16( const T* HK_RESTRICT data )
				{
					return write16( data, data->getSizeInBytes() );
				}

				/// Write \a data to a stream if \a data has getSizeInBytes() implemented and its size is NOT a
				/// multiple of 16.
				HK_FORCE_INLINE T* write( const T* HK_RESTRICT data )
				{
					return write( data, data->getSizeInBytes() );
				}

				/// Advance by \a currentNumBytes in the stream and return a pointer to storage space where
				/// \a reservedNumBytes can be written to.
				HK_FORCE_INLINE	T* advanceAndReserveNext( int currentNumBytes, int reservedNumBytes )
				{
					return (T*)hkBlockStreamBase::Writer::advanceAndReserveNext( currentNumBytes, reservedNumBytes );
				}

				/// Reserve \a numBytes in the stream. Returns a pointer where data can be written to.
				/// This method does not advance the stream automatically, you have to call advance() manually
				/// afterwards.
				HK_FORCE_INLINE	T* reserve( int numBytes )
				{
					HK_ASSERT(0xaf1e132f, hkSizeOf(T) <= numBytes);
					return (T*)hkBlockStreamBase::Writer::reserveBytes(numBytes);
				}


				/// Reserve in the stream a number of bytes equal to the sizeof of the function's template parameter. 
				/// Returns a pointer where data can be written to.
				/// This method does not advance the stream automatically, you have to call advance() manually
				/// afterwards.
				template<typename T2>
				HK_FORCE_INLINE	T2* reserve()
				{
					return (T2*)hkBlockStreamBase::Writer::reserveBytes(sizeof(T2));
				}

				/// Reserve numBytes in the stream. Returns a pointer where data can be written to.
				/// This method does not advance the stream automatically, you have to call advance() manually
				/// afterwards.
				template<typename T2>
				HK_FORCE_INLINE	T2* reserve( int numBytes )
				{
					return (T2*)hkBlockStreamBase::Writer::reserveBytes(numBytes);
				}

			
				HK_FORCE_INLINE bool isValid()
				{
					return m_blockStream != HK_NULL;
				}
		};


		/// write iterator for fixed size objects, allows for better type checking and simpler accessors
		
		struct FixedSizeWriter : public Writer
		{
		public:
			HK_FORCE_INLINE FixedSizeWriter(){}

			HK_FORCE_INLINE T* reserve(){ return Writer::reserve( sizeof(T)); }

			HK_FORCE_INLINE void advance(){ Writer::advance( sizeof(T) ); }
		};

		/// Reads a stream
		class Reader: public hkBlockStreamBase::Reader // hkBlockStreamReader
		{
		public:
			/// advance and access the next element, assumes the current entry is valid
			HK_FORCE_INLINE	const T* advanceAndAccessNext( int thisElemSize ){ return (const T*)hkBlockStreamBase::Reader::advanceAndAccessNext(thisElemSize); }

			/// advance and access the next element, assumes the current entry is valid
			template <typename T2>
			HK_FORCE_INLINE const T2* advanceAndAccessNext( int thisElemSize ){ return (const T2*)hkBlockStreamBase::Reader::advanceAndAccessNext(thisElemSize);  }

			/// advance and access the next element, assumes the current entry is valid
			template <typename T2>
			HK_FORCE_INLINE	const T2* advanceAndAccessNext( ){ return (const T2*)hkBlockStreamBase::Reader::advanceAndAccessNext(sizeof(T2));  }

			HK_FORCE_INLINE	const T* access(){ return (T*)hkBlockStreamBase::Reader::access(); }

			template <typename T2>
			HK_FORCE_INLINE	const T2* access(){ return (T2*)hkBlockStreamBase::Reader::access(); }


			/// Read from a stream if data has getSizeInBytes() implemented
			HK_FORCE_INLINE const T* advanceAndAccessNext( const T* HK_RESTRICT data ){ return advanceAndAccessNext( data->getSizeInBytes()); }
		};

		/// A reader which allows you to modify the data while reading
		class Modifier: public hkBlockStreamBase::Modifier 
		{
			public:

				HK_FORCE_INLINE	T* advanceAndAccessNext( int thisElemSize ){ return (T*)hkBlockStreamBase::Modifier::advanceAndAccessNext(thisElemSize); }

				template <typename T2>
				HK_FORCE_INLINE	T2* advanceAndAccessNext( int thisElemSize ){ return (T2*)hkBlockStreamBase::Modifier::advanceAndAccessNext(thisElemSize); }

				HK_FORCE_INLINE	T* access(){ return (T*)hkBlockStreamBase::Modifier::access(); }

				template <typename T2>
				HK_FORCE_INLINE	T2* access(){ return (T2*)hkBlockStreamBase::Modifier::access(); }
		};

		/// A reader, which frees the memory once it is fully read.
		/// You can run different Consumers using different ranges of the same block stream,
		/// even in multitple threads.
		/// Be default the blockstream becomes useless as soon as one consumer worked on it.
		/// If you want to continue to use the blockstream you have to:
		///    - finalize with a call to hkBlockStream::fixupConsumedBlocks(). This will free completely unused blocks
		class Consumer: public hkBlockStreamBase::Consumer // hkBlockStreamConsumer
		{
			public:

				/// Override to avoid calling the wrong function
				HK_FORCE_INLINE void setToRange( Allocator* allocator, const hkBlockStreamBase::Range* range ){ HK_ASSERT( 0x0f345467, false ); }	// use the other version

				/// Sets the consumer to a subrange of a stream
				HK_FORCE_INLINE void setToRange( Allocator* allocator, hkBlockStream<T>* stream, hkBlockStream<T>* streamPpu, const hkBlockStreamBase::Range* range ){ hkBlockStreamBase::Consumer::setToRange( allocator, stream, streamPpu, range ); }

				HK_FORCE_INLINE	const T* access(){ return (T*)hkBlockStreamBase::Consumer::access(); }

				/// advance and access the next element, assumes the current entry is valid
				HK_FORCE_INLINE	const T* consumeAndAccessNext( int thisElemSize ){ return (const T*)hkBlockStreamBase::Consumer::consumeAndAccessNext(thisElemSize); }

				/// Read from a stream if data has getSizeInBytes() implemented
				HK_FORCE_INLINE const T* consumeAndAccessNext( const T* HK_RESTRICT data ){ return consumeAndAccessNext( data->getSizeInBytes()); }
		};
};

#include <Common/Base/Container/BlockStream/hkBlockStream.inl>

#endif

/*
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20120831)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2012
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available at www.havok.com/tryhavok.
 * 
 */
