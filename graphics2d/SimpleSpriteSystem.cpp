/*
  SimpleSpriteSystem.cpp
  Copyright (C) David M. Anderson

  SimpleSpriteSystem class: Sprite system that redraws background and all
  sprites each frame.
*/


#include "SimpleSpriteSystem.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Random.hpp"
#include "StdClock.hpp"
#include "Circle.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


SimpleSpriteSystem::SimpleSpriteSystem( )
{
}

//-----------------------------------------------------------------------------

SimpleSpriteSystem::~SimpleSpriteSystem( )
{
}

//=============================================================================

void 
SimpleSpriteSystem::SetBackground( shared_ptr< Surface > background )
{
    m_background = background;
}

//=============================================================================

bool 
SimpleSpriteSystem::AddSprite( shared_ptr< Sprite > sprite, int level )
{
    SpriteData sd = { sprite, level };
    for ( SpriteList::iterator pSD = m_sprites.begin();
          pSD != m_sprites.end(); ++pSD )
    {
        if ( pSD->m_level > level )
        {
            m_sprites.insert( pSD, sd );
            return true;
        }
    }
    m_sprites.insert( m_sprites.end(), sd );
    return true;
}

//-----------------------------------------------------------------------------

bool 
SimpleSpriteSystem::RemoveSprite( shared_ptr< Sprite > sprite )
{
    for ( SpriteList::iterator pSD = m_sprites.begin();
          pSD != m_sprites.end(); ++pSD )
    {
        if ( pSD->m_sprite == sprite )
        {
            m_sprites.erase( pSD );
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------

bool 
SimpleSpriteSystem::SetLevel( shared_ptr< Sprite > sprite, int level )
{
    bool rmvRslt = RemoveSprite( sprite );
    if ( ! rmvRslt )
        return false;
    return AddSprite( sprite, level );
}

//=============================================================================

void 
SimpleSpriteSystem::Render( )
{
    if ( m_background )
        m_background->Blit( Point2I::Zero );
    for ( SpriteList::iterator pSD = m_sprites.begin();
          pSD != m_sprites.end(); ++pSD )
    {
        shared_ptr< Sprite > pSprite = pSD->m_sprite;
        if ( pSprite && pSprite->Graphic() && pSprite->IsVisible() )
            pSprite->Graphic()->Blit( pSprite->Position() );
    }
}

//=============================================================================

#ifdef DEBUG
//-----------------------------------------------------------------------------

bool 
SimpleSpriteSystem::Test( )
{
    bool ok = true;
    cout << "Testing SimpleSpriteSystem" << endl;

    SimpleSpriteSystem sss;

    shared_ptr< Sprite > sprites[ 10 ];
    for ( int i = 0; i < 10; ++i )
    {
        shared_ptr< Surface > pSurf;
        sprites[ i ] = shared_ptr< Sprite >(
            new Sprite( pSurf, Point2I( 2*i, 4*i ) ) );
        int lev = Random( 1000 );
        cout << "AddSprite( sprite[ " << i << " ], " << lev << " )" << endl;
        sss.AddSprite( sprites[ i ], lev );
    }
    TESTCHECK( CheckSorted( sss ), true, &ok );

    cout << "SetLevel( ) called randomly 100 times" << endl;
    for ( int i = 0; i < 100; ++i )
    {
        int s = Random( 10 );
        int lev = Random( 1000 );
        sss.SetLevel( sprites[ s ], lev );
        if ( i % 10 == 0 )
            TESTCHECK( CheckSorted( sss ), true, &ok );
    }
    
    if ( ok )
        cout << "SimpleSpriteSystem PASSED." << endl << endl;
    else
        cout << "SimpleSpriteSystem FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

bool 
SimpleSpriteSystem::CheckSorted( const SimpleSpriteSystem & sss )
{
    SpriteList::const_iterator pSpriteData0 = sss.m_sprites.begin();
    if ( pSpriteData0 == sss.m_sprites.end() )
        return true;
    SpriteList::const_iterator pSpriteData1 = sss.m_sprites.begin();
    while ( true )
    {
        if ( ++pSpriteData1 == sss.m_sprites.end() )
            return true;
        if ( pSpriteData1->m_level < pSpriteData0->m_level )
            return false;
        pSpriteData0 = pSpriteData1;
    }
}

//=============================================================================

bool 
SimpleSpriteSystem::TestUpdate( )
{
    static SimpleSpriteSystem sss;
    static vector< shared_ptr< Surface > > surfaces;
    static vector< shared_ptr< Sprite > > sprites;

    bool done = false;
    static double stageStartTime = 0.;
    static double nextUpdateTime = 0.;
    double now = StdClock::Instance().Seconds();
    if ( now >= nextUpdateTime )
    {
        nextUpdateTime = now + 1./30.;
        static int i = -1;
        static int stage = 0;
        switch ( stage )
        {
        case 0:
        {
            i = -1;
            ++stage;
            stageStartTime = now;
            break;
        }
        case 1:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    Assert( surfaces.size() == 16 );
                    Assert( sprites.size() == 16 );
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    shared_ptr< Surface > pSurf(
                        new Surface( 75, 75, PixelType8888 ) );
                    Rectangle ext = pSurf->Extent();
                    pSurf->Fill( ext, Color4B( 0, 0, 0, 0 ) );
                    Color3B color( i * 17, i * 17, 255 );
                    Circle circle( Point2I( 37, 37 ), 37 );
                    circle.Fill( color, pSurf.get() );
                    Point2I pos( i * 10 + 10, 500 );
                    surfaces.push_back( pSurf );
                    shared_ptr< Sprite > pSprite( new Sprite( pSurf, pos ) );
                    sprites.push_back( pSprite );
                    sss.AddSprite( pSprite, i );
                }
            }
            break;
        }
        case 2:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    sprites[ i ]->SetGraphic( surfaces[ 15 - i ] );
                }
            }
            break;
        }
        case 3:
        {
            double f = (now - stageStartTime) / 3.;
            Assert( f >= 0. );
            if ( f > 1. )
            {
                i = -1;
                ++stage;
                stageStartTime = now;
                f = 1.;
            }
            for ( int s = 0; s < 16; ++s )
            {
                int startX = s * 10 + 10;
                int endX = (15 - s) * 10 + 10;
                int x = startX  +  (int)(f * (endX - startX));
                int y = (int)( (.5 - fabs( f - .5 )) * (s - 7.5) * 20. ) + 500;
                sprites[ s ]->SetPosition( Point2I( x, y ) );
            }
            break;
        }
        case 4:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    sss.SetLevel( sprites[ i ], (15 - i) );
                }
            }
            break;
        }
        case 5:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    sss.SetLevel( sprites[ 15 - i ], (15 - i) );
                }
            }
            break;
        }
        case 6:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    sprites[ i ]->Hide( );
                }
            }
            break;
        }
        case 7:
        {
            int newI = (int)((now - stageStartTime) * 6.);
            Assert( newI >= 0 );
            if ( newI != i )
            {
                i = newI;
                if ( i > 15 )
                {
                    i = -1;
                    ++stage;
                    stageStartTime = now;
                }
                else
                {
                    sprites[ 15 - i ]->Show( );
                }
            }
            break;
        }
        default:
            if ( now - stageStartTime > 1. )
                done = true;
            break;
        }
    }

    sss.Render( );
    return done;
}

//-----------------------------------------------------------------------------
#endif


//*****************************************************************************
}                                                      //namespace EpsilonDelta
