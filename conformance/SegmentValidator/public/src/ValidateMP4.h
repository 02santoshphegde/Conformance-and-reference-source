/*


############################################################################
##                Test Tools Source Code License Notice                   ##
############################################################################
                 Internet Streaming Media Alliance (ISMA)

          TEST TOOLS SOURCE CODE RELATED TO THE ISMA SPECIFICATION

ISMA Member Apple ('Submitting Member'), submitting the 
Contributed Code in this Test Tools source code has agreed to the
following terms and conditions by including this notice. 'Contributed Code'
means the code originally written by, or code modifications made by, the 
Submitting Member included in this Test Tools source code.  This submission
by Apple was registered as Contribution IN009.

COMPANYNAME1, COMPANYNAME2, COMPANYNAME3, COMPANYNAME4, and COMPANYNAME5
('Previous Submitting Members'), which previously submitted contributed 
code to this Test Tools source code, have agreed to the following terms 
and conditions in their respective submissions.

--------- ISMA Members downloading and/or using this Test Tool ------------
 PLEASE READ BEFORE COPYING, INSTALLING, OR USING.  By loading or using 
 the Test Tools, you agree to the terms and conditions herein.  If you do 
 not wish to so agree, do not load, install, or use this Test Tool.  
---------------------------------------------------------------------------

1.   Terms and Conditions:

1.1  Submitting Member hereby grants to ISMA an IRREVOCABLE, world-wide,
     royalty-free, non-exclusive license:
     (a) to intellectual property rights (Copyright and Patent) in the
         Contributed Code in this Test Tools submission Licensable 
         by the Submitting Member.

        'Licensable' means having the right to grant, to the maximum
         extent possible, whether at the time of the initial grant or
         subsequently acquired, any and all of the rights conveyed herein.

     (b) to use, reproduce and DISTRIBUTE, the submitted Test Tools
         as ISMA deems appropriate.

1.2  Submitting Member hereby grants to all ISMA Members a world-wide,
     royalty-free, non-exclusive license:
     (a) to intellectual property rights (Copyright and Patent) in the
         Contributed Code in this Test Tools submission Licensable 
         by the Submitting Member.
     (b) to use, reproduce and modify the Test Tools for the non-Commercial
         Use of self-testing the ISMA Member's own products in reference
         to the ISMA specifications.

        'Commercial Use' means distribution or otherwise making the
         sumbitted source code available to a non-ISMA Member.

     (c) to make derivative works of the Test Tools that improve or extend
         the utility of the Test Tools so long as the ISMA Member that
         creates such derivative work submits it back to ISMA, subject to
         the same terms and conditions described herein.
     (d) which does not include the right to sublicense or assign, or the
         right to incorporate the Test Tools in the ISMA Member's own products.
     (e) for as long as the Member remains an ISMA Member.

1.3  Submitting Member, Previous Submitting Members and ISMA, MAKE NO WARRANTY 
     OF NONINFRINGEMENT OF THE INTELLECTUAL PROPERTY RIGHTS OF THIRD PARTIES.  
     ISMA Member agrees that ISMA, Submitting Member, and the Previous Submitting
     Members shall NOT be liable or held responsible if use of the Test Tools 
     is found to infringe the intellectual property rights of third parties.

1.4  Submitting Member represents that it holds a good faith belief that the
     Test Tools source code submitted may be of assistance to ISMA Members
     in self-testing their products with reference to the ISMA Specification.
     HOWEVER, SUBMITTING MEMBER, PREVIOUS SUBMITTING MEMBERS, AND ISMA MAKE
     NO WARRANTIES, EXPRESS OR IMPLIED AND, IN PARTICULAR, NO WARRANTY OF 
     MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. ISMA MEMBERS 
     UTILIZING THE TEST TOOLS DO SO AT THEIR OWN RISK.  SUBMITTING MEMBER, 
     PREVIOUS SUBMITTING MEMBERS, AND ISMA DISCLAIM ALL RESPONSIBILITY 
     FOR DAMAGES OR LIABILITY OF ANY KIND TO MEMBERS.

1.5  If the Submitting Member has knowledge that a license under a third 
     party's intellectual property rights may be required to exercise the
     rights granted by Submitting Member under Sections 1.1(b) or 1.2(b,c,d,e),
     the Submitting Member agrees to include a text file with the Test Tools 
     submitted titled 'LEGAL-3PARTY' which describes the claim and the party 
     making the claim in sufficient detail that a user will know whom to contact.

1.6  Submitting Member and Previous Submitting Members assume no 
     obligation to support or to update the submitted Test Tools 
     beyond the requirements of 1.5

1.7  ISMA assumes no obligation to support or to update any Test Tools and
     reserves the right to remove from the ISMA web site at any time any 
     Test Tools it finds to be infringing or otherwise problematical.
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#if defined(__GNUC__) && ( defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__) )
 #if defined(__i386__) || defined(__x86_64__) 
  #define LITTLEENDIAN 1
 #endif
#endif

#if defined(_MSC_VER) || (LITTLEENDIAN)
	#define TARGET_RT_LITTLE_ENDIAN 1
	#define TARGET_RT_BIG_ENDIAN 0
#else
	#define TARGET_RT_LITTLE_ENDIAN 0
	#define TARGET_RT_BIG_ENDIAN 1
#endif

#define true 1
#define false 0

#define TYPE_LONGLONG 1


#if defined(_MSC_VER)
	#pragma warning (disable: 4068)		// ignore unknown pragmas
	#pragma warning (disable: 4102)		// don't tell me about unreferenced labels (I like to put bail: everywhere)
#endif

typedef char *Ptr;
typedef unsigned long OSType;
typedef unsigned char Boolean;
typedef short OSErr;

#define nil 0L

#ifndef fieldOffset
	#define fieldOffset(type, field) ((short) &((type *) 0)->field)
#endif

enum {
	kSkipUnknownAtoms = 1L<<0
};

typedef unsigned char UInt8;
typedef char SInt8;
typedef long SInt32;
typedef unsigned long UInt32;
typedef long Int32;
typedef short SInt16;
typedef unsigned short UInt16;
typedef UInt32 UnsignedFixed;

typedef unsigned char uuidType[16];		// 128-bit uuid (guid)
#ifdef  _MSC_VER
typedef unsigned __int64 UInt64;
typedef __int64 SInt64;
#else
typedef unsigned long long UInt64;
typedef long long SInt64;
#endif
typedef UInt32 TimeValue;
typedef UInt32 PriorityType;
typedef SInt32 Fixed;
typedef SInt32 Fract;



typedef Fixed MatrixRecord[3][3];

enum {
	kAtomValidated = 1L<<0,
	kAtomSkipThisAtom = 1L<<1
};

typedef struct startAtomType {
	UInt32	size;
	UInt32	type;
} startAtomType;


// Limitation:  we don't handle atom's with sizes or file offsets that need more than 32-bits
typedef struct atomOffsetEntry {
	OSType 		type;			// if atomId == 'uuid', use uuid field
	uuidType 	uuid;
	UInt64 		size;			// if atomSize == 1, use longSize field
	
	UInt64 		offset;			// offset into file of atom's start	
	UInt64 		maxOffset;		// max offset into file of atom's start	

	UInt32		atomStartSize;	// size of id & size info, so it is easy to skip

	UInt32 		aoeflags;			// used for processing	
	UInt32 		refconOverride;		// used for processing	
} atomOffsetEntry;

enum {
    getAtomDispOnlyOne          = 1 << 0,
    getAtomDispIsLeaf           = 1 << 1,
    getAtomDispMustHave         = 1 << 2,
    getAtomDispGotOne           = 1 << 3
};

enum {
	noErr = 0,
	paramErr = -50,
	allocFailedErr = -2019,
	outOfDataErr = -2020,
	tooMuchDataErr = -2021,
	noCanDoErr = -2022,
	badAtomSize = -2023,
	badAtomErr = -2024,
	atomOverRunErr = -2025,
	badPublicMovieAtom = -2026,
	dataSharedErr = -2027
};

typedef struct {
	UInt8 *ptr;
	UInt32 length;
	UInt8 *cptr;
	UInt8 cbyte;
	SInt32 curbits;
	UInt32 bits_left;
	
	UInt8 prevent_emulation;	// true or false
	UInt8 emulation_position;	// 0 usually, 1 after 1 zero byte, 2 after 2 zero bytes, 3
								// after 00 00 03, and the 3 gets stripped

} BitBuffer;

OSErr BitBuffer_Init(BitBuffer *bb, UInt8 *p, UInt32 length);
UInt32 GetBits(BitBuffer *bb, UInt32 nBits, OSErr *err);


OSErr GetBytes(BitBuffer *bb, UInt32 nBytes, UInt8 *p);
UInt32 NumBytesLeft(BitBuffer *bb);
UInt32 PeekBits(BitBuffer *bb, UInt32 nBits, OSErr *err);
OSErr SkipBytes(BitBuffer *bb, UInt32 nBytes);

// ===== bit buffer video support
Boolean BitBuffer_IsVideoStartCode(BitBuffer *bb);
OSErr BitBuffer_GetVideoStartCode(BitBuffer *bb, unsigned char *outStartCode);
UInt32 read_golomb_uev(BitBuffer *bb, OSErr *errout);
SInt32 read_golomb_sev(BitBuffer *bb, OSErr *errout);
UInt32 strip_trailing_zero_bits(BitBuffer *bb, OSErr *errout);

enum {
	kMPEG4StartCode_VOS		= 0xB0,
	kMPEG4StartCode_VO		= 0xB5,

	kMPEG4StartCode_VOLMin	= 0x20,
	kMPEG4StartCode_VOLMax	= 0x2F, 

	kMPEG4StartCode_GOV		= 0xB3,
	kMPEG4StartCode_VOP		= 0xB6

};

enum {
	nal_unspec 								= 0,
	nal_slice_layer_without_partitioning 	= 1,
	nal_slice_data_partition_a_layer 		= 2,
	nal_slice_data_partition_b_layer 		= 3,
	nal_slice_data_partition_c_layer 		= 4,
	nal_slice_layer_without_partitioning_IDR = 5,
	nal_SEI 								= 6,
	nal_SPS 								= 7,
	nal_PPS 								= 8,
	nal_access_unit_delimiter				= 9,
	nal_end_of_seq							= 10,
	nal_end_of_stream						= 11,
	nal_filler_data							= 12,
	nal_SPS_Ext								= 13,
	nal_aux_slice							= 19
};

typedef char atompathType[100];
typedef char argstr[100];

void addAtomToPath( atompathType workingpath, OSType atomId, long atomIndex, atompathType curpath );
void restoreAtomPath( atompathType workingpath, atompathType curpath );


//===== Sample Table typedefs

typedef struct SampleToChunk {
    UInt32	firstChunk;
    UInt32	samplesPerChunk;
    UInt32	sampleDescriptionIndex;
} SampleToChunk;

typedef struct TimeToSampleNum {
	UInt32	sampleCount;
	TimeValue	sampleDuration;  // duration for a single sample, not total duration
} TimeToSampleNum;

typedef struct ChunkOffsetRecord {
    UInt32	chunkOffset;
} ChunkOffsetRecord;

typedef struct ChunkOffset64Record {
    UInt64	chunkOffset;
} ChunkOffset64Record;

typedef struct SampleSizeRecord {
    UInt32	sampleSize;
} SampleSizeRecord;

typedef struct SampleDescriptionHead {
	UInt32		size;
	UInt32		sdType;
	UInt32		resvd1;
	UInt16		resvdA;
	SInt16		dataRefIndex;
} SampleDescriptionHead;


typedef struct SampleDescriptionRecord {
	SampleDescriptionHead	head;
	char		data[1];
} SampleDescriptionRecord, *SampleDescriptionPtr;



//===========================

typedef struct VideoSampleDescriptionInfo {
	SInt16		version;                    /* which version is this data */
	SInt16		revisionLevel;              /* what version of that codec did this */
	UInt32		vendor;                     /* whose  codec compressed this data */
	UInt32		temporalQuality;            /* what was the temporal quality factor  */
	UInt32		spatialQuality;             /* what was the spatial quality factor */
	SInt16		width;                      /* how many pixels wide is this data */
	SInt16		height;                     /* how many pixels high is this data */
	Fixed		hRes;                       /* horizontal resolution */
	Fixed		vRes;                       /* vertical resolution */
	UInt32		dataSize;                   /* if known, the size of data for this image descriptor */
	SInt16		frameCount;                 /* number of frames this description applies to */
    char		name[32];                   /* Str31 -- name of codec ( in case not installed )  */
	SInt16		depth;                      /* what depth is this data (1-32) or ( 33-40 grayscale ) */
	SInt16		clutID;                     /* clut id or if 0 clut follows  or -1 if no clut */
	char		extensions[1];		// having trouble - sizeof this guy gives me a bad value
} VideoSampleDescriptionInfo;


// Section 8.8.8. of ISO/IEC 14496-12 4th edition

typedef struct {
    Boolean data_offset_present;
    Boolean first_sample_flags_present;
    Boolean sample_duration_present;
    Boolean sample_size_present;
    Boolean sample_flags_present;
    Boolean sample_composition_time_offsets_present;

    UInt32 version;
    UInt32 sample_count;

    UInt32 data_offset;
    UInt32 first_sample_flags;
    
    UInt32 *sample_duration;
    UInt32 *sample_size;
    UInt32 *sample_flags;
    UInt32 *sample_composition_time_offset; //Use it as a signed int when version is non-zero

} TrunInfoRec;

// Section 8.8.7. of ISO/IEC 14496-12 4th edition

typedef struct {
    UInt32    default_sample_duration;              
    UInt32    default_sample_size;                
    UInt32    default_sample_flags;               

    Boolean base_data_offset_present;             
    Boolean sample_description_index_present;     
    Boolean default_sample_duration_present;      
    Boolean default_sample_size_present;          
    Boolean default_sample_flags_present;         
    Boolean duration_is_empty;                    
    Boolean default_base_is_moof;                 

    UInt32  track_ID;                             
    UInt64  base_data_offset;                     
    UInt32  sample_description_index;             

    UInt32 numTrun;
    UInt32 processedTrun;
    TrunInfoRec *trunInfo;

    Boolean tfdtFound;
    UInt64  baseMediaDecodeTime;
    
} TrafInfoRec;


typedef struct {
    UInt32 numTrackFragments;
    UInt32 processedTrackFragments;
    TrafInfoRec *trafInfo;
} MoofInfoRec;

//===========================

typedef struct {
	OSType mediaType;
	SInt16 trackVolume;
	Fixed trackWidth;
	Fixed trackHeight;
	Fixed sampleDescWidth, sampleDescHeight;
	UInt32	trackID;
	UInt32	hintRefTrackID;

	UInt32	mediaTimeScale;
	UInt64	mediaDuration;

	//==== enough sample table information to read through the data sequentially
	UInt32 currentSampleDescriptionIndex;
	
	UInt32 sampleDescriptionCnt;		// number of sampleDescriptions
	SampleDescriptionPtr *sampleDescriptions;	// 1 based array of sample description pointers
	UInt32 *validatedSampleDescriptionRefCons;  // available for SampleDescriptionValidator to stash info for SampleValidator
	
	UInt32 sampleSizeEntryCnt;			// number of sample size entries
	UInt32 singleSampleSize;			// set if there is a constant sample size
	SampleSizeRecord *sampleSize;		// 1 based array of sample sizes

	UInt32 chunkOffsetEntryCnt;			// number of chunk offset entries
	ChunkOffset64Record *chunkOffset;	// 1 based array of chunk offsets

	UInt32 sampleToChunkEntryCnt;			// number of sampleToChunk entries
	SampleToChunk *sampleToChunk;			// 1-based array of sampleToChunk entries
	UInt32 sampleToChunkSampleSubTotal;		// total accounted for all but last entry

	UInt32 timeToSampleEntryCnt;            // number of timeToSample entries
	TimeToSampleNum *timeToSample;             // 1-based array of TimeToSampleNum entries

	UInt32 timeToSampleSampleCnt;			// number of samples described in the timeToSampleAtom
	UInt64 timeToSampleDuration;			// duration described by timeToSampleAtom (this is Total duration of all samples, 
											//   not a single sample's duration)
    UInt32    default_sample_description_index;     // Section 8.3.3. of ISO/IEC 14496-12 4th edition
    UInt32    default_sample_duration;              // Section 8.3.3. of ISO/IEC 14496-12 4th edition
    UInt32    default_sample_size;                  // Section 8.3.3. of ISO/IEC 14496-12 4th edition
    UInt32    default_sample_flags;                 // Section 8.3.3. of ISO/IEC 14496-12 4th edition

} TrackInfoRec;

int GetSampleOffsetSize( TrackInfoRec *tir, UInt32 sampleNum, UInt64 *offsetOut, UInt32 *sizeOut, UInt32 *sampleDescriptionIndexOut );
int GetChunkOffsetSize( TrackInfoRec *tir, UInt32 chunkNum, UInt64 *offsetOut, UInt32 *sizeOut, UInt32 *sampleDescriptionIndexOut );

// movie Globals
typedef struct {
    
    Boolean	fragmented;
    UInt32  numFragments;
    UInt32  processedFragments;
    UInt32  sequence_number;

	long			numTIRs;
	long			maxTIRs;
	TrackInfoRec	tirList[1];
    MoofInfoRec     *moofInfo;
} MovieInfoRec;


// enums for fileType
enum {
	filetype_mp4 = 'mp4 ',
	filetype_mp4v = 'mp4v',
	brandtype_mp41 = 'mp41',
	brandtype_isom = 'isom'
};



enum {
	checklevel_samples = 2,
	checklevel_payload = 3
};



// to validate VideoSpecificInfo and VideoProfileLevelIndication
typedef struct{
        UInt32 maxMBsec;
        UInt32 profileLevelInd;
        UInt32 vopTimeIncResolution;
        UInt32 volWidth;
        UInt32 volHeight;
} PartialVideoSC;


// Validate Globals
typedef struct {
	FILE *inFile;
	long inOffset;
	long inMaxOffset;
	
	atompathType curatompath;
	Boolean printatom; 
	Boolean printsample;
	long tabcnt;

	atomOffsetEntry *fileaoe;		// used when you need to read file & size from the file
	
	Boolean warnings;
	
	MovieInfoRec	*mir;

	// -----
	atompathType atompath;

	argstr	filetypestr;
	argstr	checklevelstr;
	argstr	samplenumberstr;
	argstr	printtypestr;

	long	filetype;
	long	checklevel;
	long	samplenumber;

	long	majorBrand;
	Boolean	brandDASH;

	Boolean	print_atompath;
	Boolean	print_atom;
	Boolean	print_fulltable;
	Boolean	print_sample;
	Boolean	print_sampleraw;
	Boolean	print_hintpayload;
	
	UInt32  visualProfileLevelIndication;// to validate if IOD corresponds to VSC


} ValidateGlobals;

extern ValidateGlobals vg;

typedef struct AtomSizeType {
	unsigned long atomSize;
	OSType atomType;
} AtomSizeType;

typedef struct AtomStartRecord {
	AtomSizeType atom;
	UInt32 versFlags;
} AtomStartRecord;

typedef OSErr (*ValidateAtomProcPtr)(OSType atomId, unsigned long atomSize, void *atomRec);
#define CallValidateAtomProc(userRoutine, atomId, atomSize, atomRec)		\
		(*(userRoutine))((atomId), (atomSize), (atomRec))

typedef struct ValidateAtomDispatch {
	OSType atomType;
	short flags;
	ValidateAtomProcPtr atomProc;
	void *getAtomRec;
	short cnt;
} ValidateAtomDispatch;

void warnprint(const char *formatStr, ...);
void errprint(const char *formatStr, ...);
void atomprint(const char *formatStr, ...);
void atomprintnotab(const char *formatStr, ...);
void atomprintdetailed(const char *formatStr, ...);
void atomprinthexdata(char *dataP, UInt32 size);
void sampleprint(const char *formatStr, ...);
void sampleprintnotab(const char *formatStr, ...);
void sampleprinthexdata(char *dataP, UInt32 size);
void sampleprinthexandasciidata(char *dataP, UInt32 size);
void toggleprintatom( Boolean onOff );
void toggleprintatomdetailed( Boolean onOff );
void toggleprintsample( Boolean onOff );
void copyCharsToStr( char *chars, char *str, UInt16 count );



//==========================================================================================



typedef struct {
	UInt64 chunkStart;
	UInt64 chunkStop;
	UInt32 trackID;
	OSType  mediaType;  
	
} chunkOverlapRec;



typedef struct AvcConfigInfo {
	AtomSizeType 	start;
	UInt8			config_ver;
	UInt8			profile;
	UInt8 			compatibility;
	UInt8			level;
	UInt8			lengthsize;
	UInt8			sps_count;
	UInt8			pps_count;
	UInt8			chroma_format;
	UInt8			bit_depth_luma_minus8;
	UInt8			bit_depth_chroma_minus8;
	UInt8			sps_ext_count;
} AvcConfigInfo;

typedef struct AvcBtrtInfo {
	AtomSizeType 	start;
	UInt32 			buffersizeDB;
	UInt32 			maxBitrate;
	UInt32			avgBitrate;
} AvcBtrtInfo;

typedef struct ColrInfo {
	AtomSizeType 	start;
	UInt32			colrtype;
	UInt16 			primaries;
	UInt16			function;
	UInt16			matrix;
} ColrInfo;

	
OSErr GetFullAtomVersionFlags( atomOffsetEntry *aoe, UInt32 *version, UInt32 *flags, UInt64 *offsetOut);

#define FOUR_CHAR_CODE(a) a

enum {
    MovieAID                    = FOUR_CHAR_CODE('moov'),
    MovieHeaderAID              = FOUR_CHAR_CODE('mvhd'),
    ClipAID                     = FOUR_CHAR_CODE('clip'),
    RgnClipAID                  = FOUR_CHAR_CODE('crgn'),
    MatteAID                    = FOUR_CHAR_CODE('matt'),
    MatteCompAID                = FOUR_CHAR_CODE('kmat'),
    TrackAID                    = FOUR_CHAR_CODE('trak'),
    UserDataAID                 = FOUR_CHAR_CODE('udta'),
    TrackHeaderAID              = FOUR_CHAR_CODE('tkhd'),
    EditsAID                    = FOUR_CHAR_CODE('edts'),
    EditListAID                 = FOUR_CHAR_CODE('elst'),
    MediaAID                    = FOUR_CHAR_CODE('mdia'),
    MediaHeaderAID              = FOUR_CHAR_CODE('mdhd'),
    MediaInfoAID                = FOUR_CHAR_CODE('minf'),
    VideoMediaInfoHeaderAID     = FOUR_CHAR_CODE('vmhd'),
    SoundMediaInfoHeaderAID     = FOUR_CHAR_CODE('smhd'),
    GenericMediaInfoHeaderAID   = FOUR_CHAR_CODE('gmhd'),
    GenericMediaInfoAID         = FOUR_CHAR_CODE('gmin'),
    DataInfoAID                 = FOUR_CHAR_CODE('dinf'),
    DataRefAID                  = FOUR_CHAR_CODE('dref'),
    SampleTableAID              = FOUR_CHAR_CODE('stbl'),
    STSampleDescAID             = FOUR_CHAR_CODE('stsd'),
    STTimeToSampAID             = FOUR_CHAR_CODE('stts'),
    STSyncSampleAID             = FOUR_CHAR_CODE('stss'),
    STSampleToChunkAID          = FOUR_CHAR_CODE('stsc'),
    STShadowSyncAID             = FOUR_CHAR_CODE('stsh'),
    HandlerAID                  = FOUR_CHAR_CODE('hdlr'),
    STSampleSizeAID             = FOUR_CHAR_CODE('stsz'),
    STSampleSize2AID            = FOUR_CHAR_CODE('stz2'),
    STChunkOffsetAID            = FOUR_CHAR_CODE('stco'),
    STChunkOffset64AID          = FOUR_CHAR_CODE('co64'),
	STSamplePadAID              = FOUR_CHAR_CODE('padb'),
    STSampleIDAID               = FOUR_CHAR_CODE('stid'),
    DataRefContainerAID         = FOUR_CHAR_CODE('drfc'),
    TrackReferenceAID           = FOUR_CHAR_CODE('tref'),
    ColorTableAID               = FOUR_CHAR_CODE('ctab'),
    LoadSettingsAID             = FOUR_CHAR_CODE('load'),
    PropertyAtomAID             = FOUR_CHAR_CODE('code'),
    InputMapAID                 = FOUR_CHAR_CODE('imap'),
    MovieBufferHintsAID         = FOUR_CHAR_CODE('mbfh'),
    MovieDataRefAliasAID        = FOUR_CHAR_CODE('mdra'),
    SoundLocalizationAID        = FOUR_CHAR_CODE('sloc'),
    CompressedMovieAID          = FOUR_CHAR_CODE('cmov'),
    CompressedMovieDataAID      = FOUR_CHAR_CODE('cmvd'),
    DataCompressionAtomAID      = FOUR_CHAR_CODE('dcom'),
    ReferenceMovieRecordAID     = FOUR_CHAR_CODE('rmra'),
    ReferenceMovieDescriptorAID = FOUR_CHAR_CODE('rmda'),
    ReferenceMovieDataRefAID    = FOUR_CHAR_CODE('rdrf'),
    ReferenceMovieVersionCheckAID = FOUR_CHAR_CODE('rmvc'),
    ReferenceMovieDataRateAID   = FOUR_CHAR_CODE('rmdr'),
    ReferenceMovieComponentCheckAID = FOUR_CHAR_CODE('rmcd'),
    ReferenceMovieQualityAID    = FOUR_CHAR_CODE('rmqu'),
    ReferenceMovieLanguageAID   = FOUR_CHAR_CODE('rmla'),
    ReferenceMovieContentRatingAID = FOUR_CHAR_CODE('rmcr'),
    ReferenceMovieCPURatingAID  = FOUR_CHAR_CODE('rmcs'),
    ReferenceMovieAlternateGroupAID = FOUR_CHAR_CODE('rmag'),
    ReferenceMovieNetworkStatusAID = FOUR_CHAR_CODE('rnet'),
    CloneMediaAID               = FOUR_CHAR_CODE('clon'),
    TrackRefAID					= FOUR_CHAR_CODE('tref'),
    HintMediaInfoHeaderAID		= FOUR_CHAR_CODE('hmhd'),
    MpegMediaInfoHeaderAID		= FOUR_CHAR_CODE('mp4s'),
    STCompTimeToSampAID			= FOUR_CHAR_CODE('ctts'),
    STDegradationPriorityAID	= FOUR_CHAR_CODE('stdp'),
    STSampleDependdencyAID		= FOUR_CHAR_CODE('sdtp'),
	UuidAID                     = FOUR_CHAR_CODE('uuid'),



    IODSAID	= FOUR_CHAR_CODE('iods')
    
};

enum {
	Class_ForbiddenTag = 0x00,
	Class_ObjectDescrTag = 0x01,
	Class_InitialObjectDescTag = 0x02,
	Class_ES_DescrTag = 0x03,
	Class_DecoderConfigDescTag = 0x04,
	Class_DecSpecificInfoTag = 0x05,
	Class_SLConfigDescrTag = 0x06,
	Class_ContentIdentDescTag = 0x07,
	Class_SupplContentIdentDescTag = 0x08,
	Class_IPI_DescPointerTag = 0x09,
	Class_IPMP_DescrPointerTag = 0x0a,
	Class_IPMP_DescTag = 0x0b,
	Class_QoS_DescrTag = 0x0c,
	Class_RegbistrationDescrTag = 0x0d,
	Class_ES_ID_IncTag = 0x0e,
	Class_ES_ID_RefTag = 0x0f,
	Class_MP4_IOD_Tag = 0x10,
	Class_MP4_OD_Tag = 0x11,
	Class_IPL_DescPointerRefTag = 0x12,
	Class_ExtendedProvileLevelDescrTag = 0x13,
	Class_profileLevelIndicationIndexDescrTag = 0x14,
	Class_Reserved1_min = 0x15,
	Class_Reserved1_max = 0x3f,
	Class_ContentClassificationDescrTag = 0x40,
	Class_KeyWordDescrTag = 0x41,
	Class_RatingDescrTag = 0x42,
	Class_LanguageDescrTag = 0x43,
	Class_ShortTextualDescrTag = 0x44,
	Class_ExpandedTextualDescrTag = 0x45,
	Class_ContentCreatorNameDescrTag = 0x46,
	Class_ContentCreationDateDescrTag = 0x47,
	Class_OCICreatorNameDescrTag = 0x48,
	Class_OCICreationDateDescrTag = 0x49,
	Class_SmpteCameraPositionTag = 0x4a,
	Class_Reserved2_min = 0x4b,
	Class_Reserved2_max = 0x5f,
	Class_Reserved3_min = 0x60,
	Class_Reserved3_max = 0xbf,
	Class_UserPrivate_min = 0xc0,
	Class_UserPrivate_max = 0xfe,
	Class_ForbiddenTag2 = 0xff
};

enum {
	ExtDescrTagStartRange = 0x80,
	ExtDescrTagEndRange = 0xfe,
	OCIDescrTagStartRange = 0x40,
	OCIDescrTagEndRange = 0x5f
};	

enum {
	Object_Systems_1 = 0x01,
	Object_Systems_2 = 0x02,
	Object_Visual_14496 = 0x20,
	Object_Visual_AVC = 0x21,
	Object_Audio_14496 = 0x40,
	Object_Unspecified = 0xFF
};

enum {
	Stream_OD = 0x01,
	Stream_ClockRef = 0x02,
	Stream_BIFS = 0x03,
	Stream_Visual = 0x04,
	Stream_Audio = 0x05,
	Stream_MPEG7 = 0x06,
	Stream_IPMP = 0x07,
	Stream_OCI = 0x08,
	Stream_MPEGJ = 0x09
};

enum {
	VSC_VO_Sequence = 0x1B0,
	VSC_VO_Sequence_end = 0x1B1,
	VSC_UserData = 0x1B2,
	VSC_GroupVOP = 0x1B3,
	VSC_ErrorCode = 0x1B4,
	VSC_VO = 0x1B5,
	VSC_VOP = 0x1B6
};

#define bitParsingSlop 4	// number of bytes extra to allocate for the bit parsing code so we never run dry

OSErr PeekDescriptorTag(BitBuffer *bb, UInt32 *tag, UInt32 *size);
OSErr GetDescriptorTagAndSize(BitBuffer *bb, UInt32 *tag, UInt32 *size);

OSErr Validate_iods_OD_Bits( Ptr dataP, unsigned long dataSize, Boolean fileForm );


int FindAtomOffsets( atomOffsetEntry *aoe, UInt64 startOffset, UInt64 maxOffset, 
			long *atomCountOut, atomOffsetEntry **atomOffsetsOut );
int GetFileDataN64( atomOffsetEntry *aoe, void *dataP, UInt64 offset64, UInt64 *newoffset64 );
int GetFileDataN32( atomOffsetEntry *aoe, void *dataP, UInt64 offset64, UInt64 *newoffset64 );
int GetFileDataN16( atomOffsetEntry *aoe, void *dataP, UInt64 offset64, UInt64 *newoffset64 );
int GetFileData( atomOffsetEntry *aoe, void *dataP, UInt64 offset64, UInt64 size64, UInt64 *newoffset64 );
int GetFileCString( atomOffsetEntry *aoe, char **strP, UInt64 offset64, UInt64 maxSize64, UInt64 *newoffset64 );
int GetFileUTFString( atomOffsetEntry *aoe, char **strP, UInt64 offset64, UInt64 maxSize64, UInt64 *newoffset64 );
int GetFileBitStreamData( atomOffsetEntry *aoe, Ptr bsDataP, UInt32 bsSize, UInt64 offset64, UInt64 *newoffset64 );
int GetFileBitStreamDataToEndOfAtom( atomOffsetEntry *aoe, Ptr *bsDataPout, UInt32 *bsSizeout, UInt64 offset64, UInt64 *newoffset64 );
int GetFileStartCode( atomOffsetEntry *aoe, UInt32 *startCode, UInt64 offset64, UInt64 *newoffset64 );

OSErr Base64DecodeToBuffer(const char *inData, UInt32 *ioEncodedLength, char *outDecodedData, UInt32 *ioDecodedDataLength);


#define BAILIFERR( statement ) do { err = statement; if (err) goto bail; } while (false)
#define BAILIFERRSET( statement ) do { statement; if (err) goto bail; } while (false)
#define BAILIF( condition, errvalue ) do { if (condition) {err = errvalue; goto bail; } } while (false)
#define BAILIFNULL( statement, errvalue ) do {if ((statement) == nil) {err = errvalue; goto bail;}} while (false)
#define BAILIFNIL( statement, errvalue ) do {if ((statement) == nil) {err = errvalue; goto bail;}} while (false)



OSErr Validate_mvhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_trak_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_iods_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_moov_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_moof_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_traf_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_dinf_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_minf_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mdia_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stbl_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mvex_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_cprt_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_loci_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_moovhnti_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_Movie_SDP( char *inSDP );

OSErr Validate_url_Entry( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_urn_Entry( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_dref_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_vmhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_smhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_hmhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mp4s_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mdhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mdia_hdlr_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_hdlr_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_tkhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tref_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_edts_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_mdia_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_stsd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stts_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_ctts_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stss_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stsc_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stsz_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stz2_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stco_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_padb_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_trex_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_mfhd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tfhd_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_trun_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tfdt_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_edts_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_elst_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_udta_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_stsh_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_stdp_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_sdtp_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr ValidateFileAtoms( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_co64_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_nmhd_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_tref_type_Atom( atomOffsetEntry *aoe, void *refcon, OSType trefType, UInt32 *firstRefTrackID );
OSErr Validate_tref_hint_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tref_dpnd_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tref_ipir_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tref_mpod_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_tref_sync_Atom( atomOffsetEntry *aoe, void *refcon );


OSErr Validate_vide_SD_Entry( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_soun_SD_Entry( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_hint_SD_Entry( atomOffsetEntry *aoe, void *refcon );


OSErr ValidateElementaryVideoStream( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_Hint_Track( atomOffsetEntry *aoe, TrackInfoRec *tir );

OSErr Validate_Random_Descriptor(BitBuffer *bb, char* dname);

OSErr Validate_uuid_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_colr_Atom( atomOffsetEntry *aoe, void *refcon );

enum { 
	kTypeAtomFlagMustHaveOne = 1<<0,
	kTypeAtomFlagCanHaveAtMostOne = 1<<1,
	kTypeAtomFlagMustBeFirst = 1<<2
};

typedef OSErr (*ValidateAtomTypeProcPtr)( atomOffsetEntry *aoe, void *refcon );
#define CallValidateAtomTypeProc(userRoutine, aoe, refcon)		\
		(*(userRoutine))((aoe),(refcon))

OSErr ValidateAtomOfType( OSType theType, long flags, ValidateAtomTypeProcPtr validateProc, 
		long cnt, atomOffsetEntry *list, void *refcon );

#define FieldMustBe( num, value, errstr ) \
	do { if ((num) != (value)) { err = badAtomErr; errprint(errstr "\n", (value), num); }} while (false)

#define FieldCheck( _condition_, errstr ) \
	do { if (!(_condition_)) { err = badAtomErr; errprint(errstr "\n"); }} while (false)

#define FieldList2(t1,t2) {t1,t2};
#define FieldList3(t1,t2,t3) {t1,t2,t3};
#define FieldList4(t1,t2,t3,t4) {t1,t2,t3,t4};
#define FieldList10(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10) {t1,t2,t3,t4,t5,t6,t7,t8,t9,t10};
#define FieldList11(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11) {t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11};

#define FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	do { _valtype_ _test_array_[] = 
#define FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ ) \
	  int num_entries = sizeof(_test_array_)/sizeof(_valtype_); \
	  int i = 0; \
	  while (i <= num_entries) { \
	  	if (i == num_entries) { \
	  		errprint(_errstr_ #_list_ "\n"); \
			i++; \
	  	} else { \
	  		if (_test_array_[i++] == _value_) break; \
	  	} \
	  } \
	 } while (false)

#define FieldMustBeOneOf2( _value_, _valtype_, _errstr_, _list_ ) \
	FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	FieldList2 _list_ \
	FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ )
#define FieldMustBeOneOf3( _value_, _valtype_, _errstr_, _list_ ) \
	FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	FieldList3 _list_ \
	FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ )
#define FieldMustBeOneOf4( _value_, _valtype_, _errstr_, _list_ ) \
	FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	FieldList4 _list_ \
	FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ )
#define FieldMustBeOneOf10( _value_, _valtype_, _errstr_, _list_ ) \
	FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	FieldList10 _list_ \
	FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ )
#define FieldMustBeOneOf11( _value_, _valtype_, _errstr_, _list_ ) \
	FieldOneOfBegin( _value_, _valtype_, _errstr_, _list_ ) \
	FieldList11 _list_ \
	FieldOneOfEnd( _value_, _valtype_, _errstr_, _list_ )

char *int64toxstr(UInt64 num);
char *int64toxstr_r(UInt64 num, char * str);
char *int64todstr(UInt64 num);
char *int64todstr_r(UInt64 num, char * str);
char *fixed16str(SInt16 num);
char *fixed16str_r(SInt16 num, char * str);
char *fixed32str(SInt32 num);
char *fixed32str_r(SInt32 num, char * str);
char *fixedU32str(UInt32 num);
char *fixedU32str_r(UInt32 num, char * str);
char *ostypetostr(UInt32 num);
char *ostypetostr_r(UInt32 num, char * buffer);
char *langtodstr(UInt16 num);
int  my_stricmp(const char* p, const char* q);

OSErr CheckMatrixForUnity( MatrixRecord mr );

OSErr Validate_vide_sample_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_soun_sample_Bitstream( BitBuffer *bb, void *refcon );

OSErr Validate_vide_ES_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_soun_ES_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_mp4s_ES_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_odsm_ES_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_sdsm_ES_Bitstream( BitBuffer *bb, void *refcon );

OSErr Validate_odsm_sample_Bitstream( BitBuffer *bb, void *refcon );
OSErr Validate_sdsm_sample_Bitstream( BitBuffer *bb, void *refcon );

typedef OSErr (*ValidateBitstreamProcPtr)( BitBuffer *bb, void *refcon );
#define CallValidateBitstreamProc(userRoutine, bb, refcon)		\
		(*(userRoutine))((bb),(refcon))


OSErr Validate_ESDAtom( atomOffsetEntry *aoe, void *refcon, ValidateBitstreamProcPtr validateBitstreamProc, char *esname );
OSErr Validate_mp4_SD_Entry( atomOffsetEntry *aoe, void *refcon, ValidateBitstreamProcPtr validateBitstreamProc, char *esname );

OSErr Validate_avcC_Atom( atomOffsetEntry *aoe, void *refcon, char *esname );
OSErr Validate_btrt_Atom( atomOffsetEntry *aoe, void *refcon, char *esname );
OSErr Validate_m4ds_Atom( atomOffsetEntry *aoe, void *refcon, char *esname );

OSErr Validate_ftyp_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_sinf_Atom( atomOffsetEntry *aoe, void *refcon, UInt32 flags );
OSErr Validate_frma_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_schm_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_schi_Atom( atomOffsetEntry *aoe, void *refcon );

OSErr Validate_meta_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_xml_Atom ( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_iloc_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_pitm_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_ipro_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_infe_Atom( atomOffsetEntry *aoe, void *refcon );
OSErr Validate_iinf_Atom( atomOffsetEntry *aoe, void *refcon );



OSErr Validate_NAL_Unit(  BitBuffer *bb, UInt8 expect_type, UInt32 nal_length );
OSErr Validate_AVCConfigRecord( BitBuffer *bb, void *refcon );

#include "EndianMP4.h"

void EndianMatrix_BtoN( MatrixRecord *matrix );
void EndianSampleDescriptionHead_BtoN( SampleDescriptionHead *sdhP );
OSErr Get_trak_Type( atomOffsetEntry *aoe, TrackInfoRec *tir );
OSErr Get_mdia_hdlr_mediaType( atomOffsetEntry *aoe, TrackInfoRec *tir );


void dispose_mir( MovieInfoRec *mir );
