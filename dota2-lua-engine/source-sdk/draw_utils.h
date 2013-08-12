// Copyright 2013 Karl Skomski - GPL v3
#pragma once

#include "source-sdk/source_sdk.h"

namespace sourcesdk {

class DrawUtils {
public:
  static DrawUtils& GetInstance() {
    static DrawUtils  instance;

    return instance;
  }

	void DrawString( int x, int y, int r, int g, int b, int a, bool center, const char *text, ... );
	void DrawRect( int x, int y, int w, int h, int r, int g, int b, int a );
	void OutlineRect( int x, int y, int w, int h, int r, int g, int b, int a );
	bool WorldToScreenNew( Vector& vIn, Vector& vOut );
  bool GetVectorInScreenSpace( Vector pos, int& iX, int& iY, Vector *vecOffset = NULL );
  int FrustumTransform( const matrix3x4_t& worldToSurface, const Vector& point, Vector& screen );
  int HudTransform( const Vector& point, Vector& screen );
private:
  DrawUtils();
	unsigned long font_;

  DrawUtils(DrawUtils const&);
  void operator=(DrawUtils const&);
};

}