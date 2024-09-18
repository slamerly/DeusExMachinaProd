#pragma once

#define kLOG_ARGS( text, ... )                  UE_LOG( LogTemp, Log, TEXT( text ), __VA_ARGS__ )
#define kERROR_LOG_ARGS( text, ... )            UE_LOG( LogTemp, Error, TEXT( text ), __VA_ARGS__ )

#define kPRINT_ARGS(index, time, color, text)	GEngine->AddOnScreenDebugMessage( index, time, color, text )
//  Print a white screen debug text for a tick
#define kPRINT_TICK(text)						kPRINT_ARGS( INDEX_NONE, 0.0f, FColor::White, text )
//  Print a white screen debug text for 5.0s
#define kPRINT(text)							kPRINT_ARGS( INDEX_NONE, 5.0f, FColor::White, text )
//  Print a selected color screen debug text for 5.0s
#define kPRINT_COLOR(text, color)				kPRINT_ARGS( INDEX_NONE, 5.0f, color, text )
//  Print a red screen debug error text for 5.0s
#define kPRINT_ERROR(text)						kPRINT_ARGS( INDEX_NONE, 5.0f, FColor::Red, FString( "ERROR : " ) + FString( text ) )
//  Print a yellow sreen debug warning text for 5.0s
#define kPRINT_WARNING(text)                    kPRINT_ARGS( INDEX_NONE, 5.0f, FColor::Yellow, FString( "WARNING: " ) + FString( text ) )

#define kSAFE_ASSERT_ARGS( expr, text, ret )    if ( !( expr ) ) { kPRINT_ERROR( text ); return ret; }
#define kSAFE_ASSERT_TEXT( expr, text )         kSAFE_ASSERT_ARGS( expr, text, )
#define kSAFE_ASSERT( expr )                    kSAFE_ASSERT_ARGS( expr, FString::Format( TEXT( "{0}::{1}: safe assert failure '{2}'" ), { __FILE__, __LINE__, #expr } ), )

//  Draw a debug line and sphere at impact from the given line trace FHitResult
#define kDEBUG_LINE_TRACE( world, result )                                                                        \
    FColor color = result.bBlockingHit ? FColor::Red : FColor::Green;                                             \
    DrawDebugLine( world, result.TraceStart, result.bBlockingHit ? result.Location : result.TraceEnd, color );    \
    DrawDebugSphere( world, result.Location, 4.0f, 4, color )