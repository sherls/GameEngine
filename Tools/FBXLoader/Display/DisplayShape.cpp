#include "DisplayShape.h"

#ifdef DUMP_FBX_INFO
void Tools::DisplayShape::DisplayShape( FbxGeometry *geometry )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nShape\n--------------------" );

	int blendShapeCount, blendShapeChannelCount, targetShapeCount;
	FbxBlendShape *blendShape;
	FbxBlendShapeChannel *blendShapeChannel;
	FbxShape *shape;

	blendShapeCount = geometry->GetDeformerCount( FbxDeformer::eBlendShape );

	for( int blendShapeIndex = 0; blendShapeIndex < blendShapeCount; blendShapeIndex++ )
	{
		blendShape = (FbxBlendShape*) geometry->GetDeformer( blendShapeIndex, FbxDeformer::eBlendShape );
		DisplayCommon::DisplayString( "    BlendShape ", (char *) blendShape->GetName() );

		blendShapeChannelCount = blendShape->GetBlendShapeChannelCount();
		for( int blendShapeChannelIndex = 0;  blendShapeChannelIndex < blendShapeChannelCount; ++blendShapeChannelIndex )
		{
			blendShapeChannel = blendShape->GetBlendShapeChannel( blendShapeChannelIndex );
			DisplayCommon::DisplayString( "    BlendShapeChannel ", (char *) blendShapeChannel->GetName() );
			DisplayCommon::DisplayDouble( "    Default Deform Value: ", blendShapeChannel->DeformPercent.Get() );

			targetShapeCount = blendShapeChannel->GetTargetShapeCount();
			for( int targetShapeIndex = 0; targetShapeIndex < targetShapeCount; targetShapeIndex++ )
			{
				shape = blendShapeChannel->GetTargetShape( targetShapeIndex );
				DisplayCommon::DisplayString( "    TargetShape ", (char *) shape->GetName() );

				int j, controlPointsCount = shape->GetControlPointsCount();
				FbxVector4 *controlPoints = shape->GetControlPoints();
				FbxLayerElementArrayTemplate<FbxVector4> *normals = NULL;
				bool status = shape->GetNormals( &normals );

				for( j = 0; j < controlPointsCount; j++ )
				{
					DisplayCommon::DisplayInt( "        Control Point ", j );
					DisplayCommon::Display3DVector( "            Coordinates: ", controlPoints[j] );

					if( status && normals && normals->GetCount() == controlPointsCount )
					{
						DisplayCommon::Display3DVector( "            Normal Vector: ", normals->GetAt(j) );
					}
				}
				//DisplayString("");
			}
		}
	}
}
#endif	// #ifdef DUMP_FBX_INFO