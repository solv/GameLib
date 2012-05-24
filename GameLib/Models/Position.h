#pragma once

#include <d3dx9.h>

class Position
{
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Dir;

public:
	Position();
	Position( const Position& );
	~Position();

	Position& operator=( const Position& );

	// ゲッター
	D3DXVECTOR3& GetPos();
	float GetPosX() const;
	float GetPosY() const;
	float GetPosZ() const;

	D3DXVECTOR3& GetDir();
	float GetDirX() const;
	float GetDirY() const;
	float GetDirZ() const;

	// セッター
	void SetPos( const D3DXVECTOR3& );
	void SetPos( const float, const float, const float ); 
	void SetPosX( float );
	void SetPosY( float );
	void SetPosZ( float );

	void SetDir( const D3DXVECTOR3& );
	void SetDir( const float, const float, const float ); 
	void SetDirX( float );
	void SetDirY( float );
	void SetDirZ( float );
};