/**
 ****************************************************************************************************
 * \file		Matrix.inl
 * \brief		The inline functions implementation of Matrix class
 ****************************************************************************************************
*/

/**
 ****************************************************************************************************
	\fn			~Matrix( void )
	\brief		Default destructor of Matrix class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Math::Matrix::~Matrix( void )
{
	if( _value )
	{
		for( UINT32 i = 0; i < _u32Row; ++i )
			delete [] _value[i];

		delete []_value;
		_value = NULL;
	}
}