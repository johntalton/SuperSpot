#include "SuperSpots.h"
#include "MySSConfigPanel.h"

#include <stdlib.h>
#include <Resources.h>
#include <View.h>

rgb_color DefaultColor = { 51,102,152,255 };

MinimalSaver::MinimalSaver( BMessage *archive, image_id image )
             :BScreenSaver( archive, image ),
             BLocker("MyScreenSaver_lock"),
             SpotSize(50),
             DoRandomSize(true),
             DoRandomColor(true),
             BlankScreen(false),
             MyColor(DefaultColor),
             DoRings(true),
             Funky(false),
             AlphaAmount(255){     
   if (!archive->IsEmpty()){
      restore_state(archive);
   }
   SetTickSize(10000);
   srandom(system_time());
} 

MinimalSaver::~MinimalSaver(void){ 
   // Nothing else to do here... 
} 

void MinimalSaver::StartConfig( BView *view ){ 
    BRect bounds = view->Bounds();
	bounds.InsetBy(5, 0);
	MySSConfigPanel *ConfigPanel = new MySSConfigPanel(bounds, this);
	view->AddChild(ConfigPanel);

} 

status_t MinimalSaver::StartSaver(BView *view, bool preview){ 
   if (preview){
      doingPreview = true;
   }else{
      doingPreview = false;
   }
   return B_OK; 
} 

void MinimalSaver::Draw(BView *view, int32 frame){ 
   BPoint center;
   BRect rect = view->Bounds();
   int size;

   if(BlankScreen && (frame == 0)){
      view->SetLowColor(0,0,0);
      view->FillRect(view->Bounds(),B_SOLID_LOW);
   }
   
   view->SetDrawingMode(B_OP_ALPHA); 
   
   if(DoRandomSize){
      size = (random() % (doingPreview ? SpotSize/4 : SpotSize));
   }else{
      size = (doingPreview ? SpotSize / 4 : SpotSize );
   }
   
   colorBase1.red = random()%255;
   colorBase1.green = random()%255;
   colorBase1.blue = random()%255;
   colorBase1.alpha = AlphaAmount;
   
   colorBase2.red = random()%255;
   colorBase2.green = random()%255;
   colorBase2.blue = random()%255;
   colorBase2.alpha = AlphaAmount;
   
   MyColor.alpha = AlphaAmount;
   
   center = BPoint(random() % (int16)rect.Width(), random() % (int16)rect.Height());
   if(DoRings){
      view->SetHighColor(colorBase1);
      view->StrokeEllipse(center, size, size);
   }
   if(DoRandomColor){
      view->SetHighColor(colorBase2);   
   }else{
      view->SetHighColor(MyColor);
   }
   if(Funky){
      pattern mypattern;
	  for( int k = 0; k < 8; k++){
	     mypattern.data[k] = rand() % 256;
	  }
      view->SetDrawingMode(B_OP_COPY); // Some reason cant do patter and alpha?
      view->FillEllipse(center, size-3, size-3, mypattern);
      view->SetDrawingMode(B_OP_ALPHA);
   }else{
      view->FillEllipse(center, size - 3, size - 3);
   }
}

void MinimalSaver::StopSaver(void){
}

status_t MinimalSaver::SaveState(BMessage *state_msg) const{
   state_msg->AddInt32("spotsize", SpotSize);
   state_msg->AddBool("random", DoRandomSize);
   state_msg->AddBool("randcolor",DoRandomColor);
   state_msg->AddBool("blankscr",BlankScreen);
   state_msg->AddInt8("colorR",MyColor.red);
   state_msg->AddInt8("colorG",MyColor.green);
   state_msg->AddInt8("colorB",MyColor.blue);
   state_msg->AddBool("ring",DoRings);
   state_msg->AddBool("funky",Funky);
   state_msg->AddInt32("alphavalue", AlphaAmount);
   return B_OK;
}


void MinimalSaver::restore_state(BMessage *state_msg){
   int32 new_spotsize;
   bool new_randsize;
   bool new_randcolor;
   bool new_blankscr;
   int8 temp;
   bool new_rings;
   bool new_funky;
   int32 new_alpha;
   
   if (state_msg->FindInt32("spotsize", &new_spotsize) == B_OK)
      SpotSize = new_spotsize;
   if (state_msg->FindBool("random", &new_randsize) == B_OK)
      DoRandomSize = new_randsize;
   if (state_msg->FindBool("randcolor", &new_randcolor) == B_OK)
      DoRandomColor = new_randcolor;
   if (state_msg->FindBool("blankscr", &new_blankscr) == B_OK)
      BlankScreen = new_blankscr;
   if (state_msg->FindInt8("colorR",&temp) == B_OK)
      MyColor.red = temp;
   if (state_msg->FindInt8("colorG",&temp) == B_OK)
      MyColor.green = temp;
   if (state_msg->FindInt8("colorB",&temp) == B_OK)
      MyColor.blue = temp;
   if (state_msg->FindBool("ring", &new_rings) == B_OK)
      DoRings = new_rings;
   if (state_msg->FindBool("funky", &new_funky) == B_OK)
      Funky = new_funky;
   if (state_msg->FindInt32("alphavalue", &new_alpha) == B_OK)
      AlphaAmount = new_alpha;
   
}


extern "C" _EXPORT
BScreenSaver *instantiate_screen_saver( BMessage *msg, image_id id ){ 
    return new MinimalSaver( msg, id ); 
} 
