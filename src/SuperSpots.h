#ifndef MY_SAVER
#define MY_SAVER
#include <ScreenSaver.h>

#include <Locker.h>

class BSlider;
class MinimalSaver : public BScreenSaver, public BLocker {  
public: 
   MinimalSaver( BMessage *archive, image_id image ); 
   ~MinimalSaver( void ); 
    
   //status_ InitCheck{ return B_OK; }
   void StartConfig( BView *view ); 
   status_t StartSaver( BView *view, bool preview ); 
   status_t SaveState(BMessage *state_msg) const;
   void	restore_state(BMessage *state_msg);
   void Draw( BView *view, int32 frame ); 
   void StopSaver(void);
   
   rgb_color colorBase1;
   rgb_color colorBase2;
   rgb_color colorCurr1;
   rgb_color colorCurr2; 
   bool doingPreview;
   int32 SpotSize;
   bool DoRandomSize;
   bool DoRandomColor;
   bool BlankScreen;
   rgb_color MyColor;
   bool DoRings;
   bool Funky;
   int32 AlphaAmount;

}; 
#endif