#include "Position.h"

Position::Position():
	m_Pos( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) ),
	m_Dir( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )
{
}

Position::Position( const Position& obj ){
	m_Pos = obj.m_Pos;
	m_Dir = obj.m_Dir;
}

Position::~Position(){
}

Position& Position::operator=( const Position& obj ){
	m_Pos = obj.m_Pos;
	m_Dir = obj.m_Dir;
	return *this;
}

D3DXVECTOR3& Position::GetPos(){
	return m_Pos;
}
float Position::GetPosX() const{
	return m_Pos.x;
}
float Position::GetPosY() const{
	return m_Pos.y;
}
float Position::GetPosZ() const{
	return m_Pos.z;
}

D3DXVECTOR3& Position::GetDir(){
	return m_Dir;
}
float Position::GetDirX() const{
	return m_Dir.x;
}
float Position::GetDirY() const{
	return m_Dir.y;
}
float Position::GetDirZ() const{
	return m_Dir.z;
}

void Position::SetPos( const D3DXVECTOR3& obj ){
	m_Pos = obj;
}
void Position::SetPos( const float x_, const float y_, const float z_ ){
	m_Pos.x = x_;
	m_Pos.y = y_;
	m_Pos.z = z_;
}
void Position::SetPosX( const float x_ ){
	m_Pos.x = x_;
}
void Position::SetPosY( const float y_ ){
	m_Pos.y = y_;
}
void Position::SetPosZ( const float z_ ){
	m_Pos.z = z_;
}

void Position::SetDir( const D3DXVECTOR3& obj ){
	m_Dir = obj;
}
void Position::SetDir( const float x_, const float y_, const float z_ ){
	m_Dir.x = x_;
	m_Dir.y = y_;
	m_Dir.z = z_;
}
void Position::SetDirX( const float x_ ){
	m_Dir.x = x_;
}
void Position::SetDirY( const float y_ ){
	m_Dir.y = y_;
}
void Position::SetDirZ( const float z_ ){
	m_Dir.z = z_;
}