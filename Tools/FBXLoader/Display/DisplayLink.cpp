#include "DisplayLink.h"

#ifdef DUMP_FBX_INFO

void Tools::DisplayLink::DisplayLink( FbxGeometry *i_geometry )
{
	DisplayCommon::DisplayString( "\n\n--------------------\nLink\n--------------------" );

	int i, j;
	int skinCount = 0;
	int clusterCount = 0;
	FbxCluster *cluster;

	skinCount = i_geometry->GetDeformerCount( FbxDeformer::eSkin );

	//int linkCount = i_geometry->GetLinkCount();
	for( i = 0; i < skinCount; i++ )
	{
		clusterCount = ((FbxSkin *) i_geometry->GetDeformer( i, FbxDeformer::eSkin) )->GetClusterCount();
		for( j = 0; j != clusterCount; j++ )
		{
			DisplayCommon::DisplayInt( "    Cluster ", i );

			cluster = ((FbxSkin *)i_geometry->GetDeformer( i, FbxDeformer::eSkin) )->GetCluster( j );
			//lLink = pGeometry->GetLink(i);

			const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

			DisplayCommon::DisplayString( "    Mode: ", lClusterModes[cluster->GetLinkMode()] );

			if( cluster->GetLink() != NULL )
			{
				DisplayCommon::DisplayString( "        Name: ", (char *) cluster->GetLink()->GetName() );
			}

			FbxString string1 = "        Link Indices: ";
			FbxString string2 = "        Weight Values: ";

			int k, indexCount = cluster->GetControlPointIndicesCount();
			int *indices = cluster->GetControlPointIndices();
			double *lWeights = cluster->GetControlPointWeights();

			for( k = 0; k < indexCount; k++ )
			{
				string1 += indices[k];
				string2 += (float) lWeights[k];

				if( k < indexCount - 1 )
				{
					string1 += ", ";
					string2 += ", ";
				}
			}

			DisplayCommon::DisplayString( string1 );
			DisplayCommon::DisplayString( string2 );

			string1 += "\n";
			string2 += "\n";

			FBXSDK_printf( string1 );
			FBXSDK_printf( string2 );

			DisplayCommon::DisplayString( "" );

			FbxAMatrix matrix;

			matrix = cluster->GetTransformMatrix( matrix );
			DisplayCommon::Display3DVector( "        Transform Translation: ", matrix.GetT() );
			DisplayCommon::Display3DVector( "        Transform Rotation: ", matrix.GetR() );
			DisplayCommon::Display3DVector( "        Transform Scaling: ", matrix.GetS() );

			matrix = cluster->GetTransformLinkMatrix( matrix);
			DisplayCommon::Display3DVector( "        Transform Link Translation: ", matrix.GetT() );
			DisplayCommon::Display3DVector( "        Transform Link Rotation: ", matrix.GetR() );
			DisplayCommon::Display3DVector( "        Transform Link Scaling: ", matrix.GetS() );

			if( cluster->GetAssociateModel() != NULL )
			{
				matrix = cluster->GetTransformAssociateModelMatrix( matrix );
				DisplayCommon::DisplayString( "        Associate Model: ", (char *) cluster->GetAssociateModel()->GetName() );
				DisplayCommon::Display3DVector( "        Associate Model Translation: ", matrix.GetT() );
				DisplayCommon::Display3DVector( "        Associate Model Rotation: ", matrix.GetR() );
				DisplayCommon::Display3DVector( "        Associate Model Scaling: ", matrix.GetS() );
			}

			DisplayCommon::DisplayString( "" );
		}
	}
}
#endif	// #ifdef DUMP_FBX_INFO