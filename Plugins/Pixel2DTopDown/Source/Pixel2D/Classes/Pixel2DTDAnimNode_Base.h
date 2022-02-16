//Polyart Studio 2021
 

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "Animation/AnimNodeBase.h"
#include "PaperFlipbook.h"
#include "Pixel2DTDAnimNode_Base.generated.h"

class UPixel2DTDAnimBlueprint;
class UPixel2DTDAnimInstance;
class UPixel2DTDAnimBlueprintGeneratedClass;
struct FPixel2DTDAnimInstanceProxy;
struct FPixel2DTDAnimNode_Base;
struct FPixel2DTDAnimNotifyQueue;

/** Base class for update/evaluate contexts */
struct FPixel2DTDAnimationBaseContext
{
public:
	FPixel2DTDAnimInstanceProxy* AnimInstanceProxy;

protected:
	PIXEL2DTD_API FPixel2DTDAnimationBaseContext(FPixel2DTDAnimInstanceProxy* InAnimInstanceProxy);

public:
	// Get the Blueprint IAnimClassInterface associated with this context, if there is one.
	// Note: This can return NULL, so check the result.
	PIXEL2DTD_API UPixel2DTDAnimBlueprintGeneratedClass* GetAnimClass() const;
};

/** Initialization context passed around during animation tree initialization */
struct FPixel2DTDAnimationInitializeContext : public FPixel2DTDAnimationBaseContext
{
public:
	FPixel2DTDAnimationInitializeContext(FPixel2DTDAnimInstanceProxy* InAnimInstanceProxy)
		: FPixel2DTDAnimationBaseContext(InAnimInstanceProxy)
	{
	}
};

struct FPixel2DTDAnimationUpdateContext : public FPixel2DTDAnimationBaseContext
{
	float DeltaTime;
	FPixel2DTDAnimNotifyQueue *NotifyQueue;
public:
	FPixel2DTDAnimationUpdateContext(FPixel2DTDAnimInstanceProxy* InAnimInstanceProxy, float InDeltaTime, FPixel2DTDAnimNotifyQueue * NotifyQueue)
		: FPixel2DTDAnimationBaseContext(InAnimInstanceProxy)
		, DeltaTime(InDeltaTime)
		, NotifyQueue(NotifyQueue)
	{
	}

	// Returns the delta time for this update, in seconds
	float GetDeltaTime() const { return DeltaTime; }
};


/** Evaluation context passed around during animation tree evaluation */
struct FPixel2DTDPoseContext : public FPixel2DTDAnimationBaseContext
{
public:
	UPaperFlipbook *Flipbook;

public:
	// This constructor allocates a new uninitialized pose for the specified anim instance
	FPixel2DTDPoseContext(FPixel2DTDAnimInstanceProxy* InAnimInstanceProxy)
		: FPixel2DTDAnimationBaseContext(InAnimInstanceProxy)
	{
		Initialize(InAnimInstanceProxy);
	}

	// This constructor allocates a new uninitialized pose, copying non-pose state from the source context
	FPixel2DTDPoseContext(const FPixel2DTDPoseContext& SourceContext)
		: FPixel2DTDAnimationBaseContext(SourceContext.AnimInstanceProxy)
	{
		Initialize(SourceContext.AnimInstanceProxy);
	}

	PIXEL2DTD_API void Initialize(FPixel2DTDAnimInstanceProxy* InAnimInstanceProxy);
};


/** A pose link to another node */
USTRUCT(BlueprintInternalUseOnly)
struct PIXEL2DTD_API FPixel2DTDPoseLinkBase
{
	GENERATED_USTRUCT_BODY()

	/** Serialized link ID, used to build the non-serialized pointer map. */
	UPROPERTY()
	int32 LinkID;

#if WITH_EDITORONLY_DATA
	/** The source link ID, used for debug visualization. */
	UPROPERTY()
	int32 SourceLinkID;
#endif

protected:
	/** The non serialized node pointer. */
	struct FPixel2DTDAnimNode_Base* LinkedNode;

	/** Flag to prevent reentry when dealing with circular trees. */
	bool bProcessed;

public:
	FPixel2DTDPoseLinkBase()
		: LinkID(INDEX_NONE)
#if WITH_EDITORONLY_DATA
		, SourceLinkID(INDEX_NONE)
#endif
		, LinkedNode(NULL)
		, bProcessed(false)
	{
	}

	// Interface
	void Initialize(const FPixel2DTDAnimationInitializeContext& Context);
	void Update(const FPixel2DTDAnimationUpdateContext& Context);

	/** Try to re-establish the linked node pointer. */
	void AttemptRelink(const FPixel2DTDAnimationBaseContext& Context);

};

USTRUCT(BlueprintInternalUseOnly)
struct PIXEL2DTD_API FPixel2DTDPoseLink : public FPixel2DTDPoseLinkBase
{
	GENERATED_USTRUCT_BODY()

public:
	// Interface
	void Evaluate(FPixel2DTDPoseContext& Output);
};

UENUM()
enum class ECopyTypeTD : uint8
{
	// For plain old data types, we do a simple memcpy.
	PlainProperty,

	// Read and write properties using bool property helpers, as source/dest could be bitfield or boolean
	BoolProperty,

	// Use struct copy operation, as this needs to correctly handle CPP struct ops
	StructProperty,

	// Read and write properties using object property helpers, as source/dest could be regular/weak/lazy etc.
	ObjectProperty,

	// FName needs special case because its size changes between editor/compiler and runtime.
	NameProperty,
};



USTRUCT()
struct FPixel2DTDExposedValueCopyRecord
{
    GENERATED_USTRUCT_BODY()

		FPixel2DTDExposedValueCopyRecord()
        : SourcePropertyName(NAME_None)
        , SourceSubPropertyName(NAME_None)
        , SourceArrayIndex(0)
        , DestProperty(nullptr)
        , DestArrayIndex(0)
        , Size(0)
        , bInstanceIsTarget(false)
        , PostCopyOperation(EPostCopyOperation::None)
        , CopyType(ECopyTypeTD::PlainProperty)
        , CachedSourceProperty(nullptr)
        , CachedSourceContainer(nullptr)
        , CachedDestContainer(nullptr)
        , Source(nullptr)
        , Dest(nullptr)
    {}

    UPROPERTY()
    FName SourcePropertyName;

    UPROPERTY()
    FName SourceSubPropertyName;

    UPROPERTY()
    int32 SourceArrayIndex;

    UPROPERTY()
	TFieldPath<FProperty> DestProperty;

    UPROPERTY()
    int32 DestArrayIndex;

    UPROPERTY()
    int32 Size;

   // Whether or not the anim instance object is the target for the copy instead of a node.
    UPROPERTY()
    bool bInstanceIsTarget;

    UPROPERTY()
    EPostCopyOperation PostCopyOperation;

    UPROPERTY(Transient)
    ECopyTypeTD CopyType;

    // cached source property
    UPROPERTY(Transient)
	TFieldPath<FProperty> CachedSourceProperty;

    // cached source container for use with boolean operations
    void* CachedSourceContainer;

    // cached dest container for use with boolean operations
    void* CachedDestContainer;

    // Cached source copy ptr
    void* Source;

    // Cached dest copy ptr
    void* Dest;
};


// An exposed value updater
USTRUCT()
struct PIXEL2DTD_API FPaper2DTDExposedValueHandler
{
	GENERATED_USTRUCT_BODY()

	FPaper2DTDExposedValueHandler()
        : BoundFunction(NAME_None)
        , Function(nullptr)
        , bInitialized(false)
	{
	}

    // The function to call to update associated properties (can be NULL)
    UPROPERTY()
    FName BoundFunction;

    // Direct data access to property in anim instance
    UPROPERTY()
    TArray<FPixel2DTDExposedValueCopyRecord> CopyRecords;

    // function pointer if BoundFunction != NAME_None
    UFunction* Function;

    // Prevent multiple initialization
    bool bInitialized;

	// Bind copy records and cache UFunction if necessary
	void Initialize(FPixel2DTDAnimNode_Base* AnimNode, UObject* AnimInstanceObject);

	// Execute the function and copy records
	void Execute(const FPixel2DTDAnimationBaseContext& Context) const;
};

/**
* This is the base of all runtime animation nodes
*
* To create a new animation node:
*   Create a struct derived from FPixel2DTDAnimNode_Base - this is your runtime node
*   Create a class derived from UPixel2DTDAnimGraphNode_Base, containing an instance of your runtime node as a member - this is your visual/editor-only node
*/
USTRUCT()
struct PIXEL2DTD_API FPixel2DTDAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	// The default handler for graph-exposed inputs
	UPROPERTY(meta = (BlueprintCompilerGeneratedDefaults))
	FPaper2DTDExposedValueHandler EvaluateGraphExposedInputs;

	/**
	* Called when the node first runs. If the node is inside a state machine or cached pose branch then this can be called multiple times.
	* This can be called on any thread.
	* @param	Context		Context structure providing access to relevant data
	*/
	virtual void Initialize_AnyThread(const FPixel2DTDAnimationInitializeContext& Context);

	/**
	* Called to update the state of the graph relative to this node.
	* Generally this should configure any weights (etc.) that could affect the poses that
	* will need to be evaluated. This function is what usually executes EvaluateGraphExposedInputs.
	* This can be called on any thread.
	* @param	Context		Context structure providing access to relevant data
	*/
	virtual void Update_AnyThread(const FPixel2DTDAnimationUpdateContext& Context);

	virtual void Evaluate_AnyThread(FPixel2DTDPoseContext& Output);

    /** Called after compilation */
	virtual void PostCompile() {}

	virtual ~FPixel2DTDAnimNode_Base() {}
};