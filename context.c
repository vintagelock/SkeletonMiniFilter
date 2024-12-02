
#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

#include "context.h"


//////////////////////////////////////////////////////////////////////////

VOID CtxCleanupCallback(IN PFLT_CONTEXT Context, IN FLT_CONTEXT_TYPE ContextType)
{
	switch (ContextType)
	{
		case FLT_INSTANCE_CONTEXT:
		{
			PSKEL_INSTANCE_CONTEXT ctx = (PSKEL_INSTANCE_CONTEXT)Context;

			ExDeleteResourceLite(&ctx->Resource);
		}
		break;

		case FLT_STREAM_CONTEXT:
		{
			PSKEL_STREAM_CONTEXT ctx = (PSKEL_STREAM_CONTEXT)Context;

			ExDeleteResourceLite(&ctx->Resource);
		}
		break;

		case FLT_STREAMHANDLE_CONTEXT:
		{
			PSKEL_STREAM_HANDLE_CONTEXT ctx = (PSKEL_STREAM_HANDLE_CONTEXT)Context;

			ExDeleteResourceLite(&ctx->Resource);
		}
		break;

		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
