#include "MySSConfigPanel.h"
#include "SuperSpots.h"

#include <Slider.h>
#include <CheckBox.h>
#include <ColorControl.h>
#include <StringView.h>
#include <Alert.h>
#include <MenuItem.h>
#include <MenuField.h>

rgb_color Black = {0,0,0,0};
rgb_color White = {255,255,255,0};
rgb_color Default = { 51,102,152,255 };
rgb_color LightBlue = { 155,155,250 };

MySSConfigPanel::MySSConfigPanel(BRect frame, MinimalSaver *Ssaver)
	: BView(frame, "", B_FOLLOW_NONE, B_WILL_DRAW),
	  saver(Ssaver)
{

   const BRect bounds = Bounds();
   const BPoint lt = bounds.LeftTop();
   BRect t_bounds;  

   // Size slider
   rgb_color green = { 0,255,0,255 };
   rgb_color darkgreen = { 0,180,0,255 };
   BRect sliderect(BPoint(bounds.left,0), BPoint(bounds.right, lt.y));
   size_slider = new BSlider(sliderect, "speed", "Max Spot Size",
                 new BMessage('size'),
	             4, 100, B_BLOCK_THUMB, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW|B_NAVIGABLE); 
   size_slider->SetLimitLabels("Tiny", "Huge");
   size_slider->SetHashMarks(B_HASH_MARKS_BOTTOM);
   size_slider->SetHashMarkCount(20);
   size_slider->SetKeyIncrementValue(5);
   size_slider->SetBarColor(darkgreen);
   size_slider->UseFillColor(true, &green);
   size_slider->SetValue(saver->SpotSize);
   AddChild(size_slider);

   t_bounds = size_slider->Frame();
      
   // Should we do random sizes checkbox
   randomize = new BCheckBox(BRect(0,t_bounds.bottom+4,t_bounds.right,10), "rand_s", "Random Size",new BMessage('rand'));
   AddChild(randomize);
   randomize->ResizeToPreferred();
   if (saver->DoRandomSize){ randomize->SetValue(B_CONTROL_ON); }
  
   t_bounds = randomize->Frame();

   blank = new BCheckBox(BRect(bounds.right/2+5,t_bounds.top,bounds.right,5), "blank", "Blank Screen first",new BMessage('blnk'));
   AddChild(blank);
   blank->ResizeToPreferred();
   if (saver->BlankScreen){ blank->SetValue(B_CONTROL_ON); }

   // Ramdom color ... or not
   randcolor = new BCheckBox(BRect(0,t_bounds.bottom+4,bounds.right,5), "rand_c", "Use Random Colors",new BMessage('racl'));
   AddChild(randcolor);
   randcolor->ResizeToPreferred();
   if (saver->DoRandomColor){ randcolor->SetValue(B_CONTROL_ON); }
 
   t_bounds = randcolor->Frame();   
    
   // Should we draw nice little circles around our dots
   rings = new BCheckBox(BRect(bounds.right/2+5,t_bounds.top,bounds.right,5), "ring", "Rings around dots",new BMessage('ring'));
   AddChild(rings);
   rings->ResizeToPreferred();
   if (saver->DoRings){ rings->SetValue(B_CONTROL_ON); }

  
//   t_bounds = rings->Frame();

   // Color picker.. hehe  fun stuff
   Cpicker = new BColorControl(BPoint(0,t_bounds.bottom+4),B_CELLS_32x8,(float)1.0,"Pick",new BMessage('colr'),false);
   Cpicker->SetValue(saver->MyColor);
   AddChild(Cpicker);
    
   t_bounds = Cpicker->Frame(); 

   // Alpa slider
   alpha_slider = new BSlider(BRect(0,t_bounds.bottom+4,bounds.right,10),
                 "alpha", "Alpha Values", new BMessage('alha'),
	             0, 255, B_BLOCK_THUMB, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW|B_NAVIGABLE); 
   alpha_slider->SetLimitLabels("Transparent", "Opaque");
   alpha_slider->SetHashMarks(B_HASH_MARKS_BOTTOM);
   alpha_slider->SetHashMarkCount(20);
   alpha_slider->SetKeyIncrementValue(5);
   alpha_slider->SetBarColor(darkgreen);
   alpha_slider->UseFillColor(true, &green);
   alpha_slider->SetValue(saver->AlphaAmount);
   AddChild(alpha_slider);

   t_bounds = alpha_slider->Frame();

   // Should we be in that nice patter mode called Funky
   funk = new BCheckBox(BRect(0,bounds.bottom -35,bounds.right,bounds.bottom), "funk", "Funky mode",new BMessage('funk'));
   AddChild(funk);
   funk->ResizeToPreferred();
   if (saver->Funky){ funk->SetValue(B_CONTROL_ON); }

   t_bounds = funk->Frame();

   Fmenu = new BMenu("Favorits");
   
   Fmenu->AddItem(item=new BMenuItem("Add Fav", NULL));
   item->SetEnabled(false);
   Fmenu->AddSeparatorItem();
   Fmenu->AddItem(item=new BMenuItem("Standard Spots", new BMessage('Fstd'))); 
   Fmenu->AddItem(item=new BMenuItem("Funky Spots", new BMessage('Ffuk')));
   Fmenu->AddItem(item=new BMenuItem("Defalt", new BMessage('Fdft')));
   Fmenu->AddItem(item=new BMenuItem("Clouds", new BMessage('Fcld')));
   Fmenu->AddItem(item=new BMenuItem("Rings", new BMessage('Frng')));
   Fmenu->AddItem(item=new BMenuItem("Rain", new BMessage('Fran')));


   BMenuField *favmenu = new BMenuField(BRect(bounds.right-75,bounds.bottom-35,bounds.bottom-20,bounds.right),"","",Fmenu);
   favmenu->SetDivider(2);
   AddChild(favmenu);

   // Some about stuff .. .:)
   //about = new
   AddChild(new BStringView(BRect(0,bounds.bottom -10,bounds.right,bounds.bottom),B_EMPTY_STRING, 
          "SuperSpots 4.5.1 by YNOP(ynop@acm.org)"));   
}


void MySSConfigPanel::AttachedToWindow(void){
   rgb_color view_color(Parent()->ViewColor());
   SetViewColor(view_color);
  // rgb_color green = { 0,255,0,255 };
  // SetViewColor(green);
   size_slider->SetTarget(this);
   randomize->SetTarget(this);
   blank->SetTarget(this);
   randcolor->SetTarget(this);
   Cpicker->SetTarget(this);
   rings->SetTarget(this);
   funk->SetTarget(this);
   alpha_slider->SetTarget(this);
  // Fmenu->SetTarget(this);
   for(int32 i=0;i<Fmenu->CountItems();i++){
      (Fmenu->ItemAt(i))->SetTarget(this);
   }
   window_flags = Window()->Flags();
   Window()->SetFlags(window_flags | B_ASYNCHRONOUS_CONTROLS);
}


void MySSConfigPanel::DetachedFromWindow(void){
   Window()->SetFlags(window_flags);
}

void MySSConfigPanel::UpdatePrefs(void){
   size_slider->SetValue(saver->SpotSize);
   if (saver->DoRandomSize){ randomize->SetValue(B_CONTROL_ON); }
   if (saver->BlankScreen){ blank->SetValue(B_CONTROL_ON); }
   if (saver->DoRandomColor){ randcolor->SetValue(B_CONTROL_ON); }
   if (saver->DoRings){ rings->SetValue(B_CONTROL_ON); }
   Cpicker->SetValue(saver->MyColor);
   alpha_slider->SetValue(saver->AlphaAmount);
   if (saver->Funky){ funk->SetValue(B_CONTROL_ON); }
}

void MySSConfigPanel::MessageReceived(BMessage *msg){
   switch (msg->what) {
      case 'size':
         saver->Lock();
         saver->SpotSize = size_slider->Value();
         saver->Unlock();
      case 'rand':
         saver->Lock();
         if (randomize->Value() == B_CONTROL_ON)
            saver->DoRandomSize = true;
         else
            saver->DoRandomSize = false;
         saver->Unlock();
         break;
      case 'racl':
         saver->Lock();
         if (randcolor->Value() == B_CONTROL_ON)
            saver->DoRandomColor = true;
         else
            saver->DoRandomColor = false;
         saver->Unlock();
         break;
      case 'blnk':
         saver->Lock();
         if (blank->Value() == B_CONTROL_ON)
            saver->BlankScreen = true;
         else
            saver->BlankScreen = false;
         saver->Unlock();
         break;
      case 'colr':
         saver->Lock();
         saver->MyColor = Cpicker->ValueAsColor();
         saver->Unlock();
         break;
      case 'ring':
         saver->Lock();
         if (rings->Value() == B_CONTROL_ON)
            saver->DoRings = true;
         else
            saver->DoRings = false;
         saver->Unlock();
         break;
      case 'funk':
         saver->Lock();
         if (funk->Value() == B_CONTROL_ON){
            (new BAlert("","Alpha is not supported while in pattern mode.\nDon\'t ask me why!","Oh"))->Go();
            saver->Funky = true;
         }else{
            saver->Funky = false;
         }
         saver->Unlock();
         break;
      case 'alha':
         saver->Lock();
         saver->AlphaAmount = alpha_slider->Value();
         saver->Unlock();
         break;
      case 'Fstd':
         saver->Lock();
         saver->SpotSize = 20;
         saver->DoRandomSize = false;
         saver->DoRandomColor = false;
         saver->BlankScreen = false;
         saver->MyColor = Black;
         saver->DoRings = false;
         saver->Funky = false;        
         saver->AlphaAmount = 255; 
         UpdatePrefs();
         saver->Unlock();
         break;
      case 'Ffuk':
         saver->Lock();
         saver->SpotSize = 50;
         saver->DoRandomSize = true;
         saver->DoRandomColor = false;
         saver->BlankScreen = false;
         saver->MyColor = White;
         saver->DoRings = false;
         saver->Funky = true;
         saver->AlphaAmount = 255; 
         UpdatePrefs();        
         saver->Unlock();
         break;
      case 'Fcld':
         saver->Lock();
         saver->SpotSize = 50;
         saver->DoRandomSize = true ;
         saver->DoRandomColor = false;
         saver->BlankScreen = true;
         saver->MyColor = White;
         saver->DoRings = false;
         saver->Funky = false;
         saver->AlphaAmount = 20; 
         UpdatePrefs();        
         saver->Unlock();
         break;
      case 'Frng':
         saver->Lock();
         saver->SpotSize = 60;
         saver->DoRandomSize = true;
         saver->DoRandomColor = false;
         saver->BlankScreen = true;
         saver->MyColor = Black;
         saver->DoRings = true;
         saver->Funky = false;
         saver->AlphaAmount = 30; 
         UpdatePrefs();        
         saver->Unlock();
         break;
      case 'Fdft':
         saver->Lock();
         saver->SpotSize = 50;
         saver->DoRandomSize = true;
         saver->DoRandomColor = true;
         saver->BlankScreen = false;
         saver->MyColor = Default;
         saver->DoRings = true;
         saver->Funky = false;
         saver->AlphaAmount = 255;         
         saver->Unlock();
         UpdatePrefs();
         break;
      case 'Fran':
         saver->Lock();
         saver->SpotSize = 15;
         saver->DoRandomSize = true;
         saver->DoRandomColor = false;
         saver->BlankScreen = false;
         saver->MyColor = LightBlue;
         saver->DoRings = false;
         saver->Funky = false;
         saver->AlphaAmount = 100;   
         UpdatePrefs();      
         saver->Unlock();
         break;
      /*case '':
         saver->Lock();
         saver->SpotSize = ;
         saver->DoRandomSize = ;
         saver->DoRandomColor = ;
         saver->BlankScreen = ;
         saver->MyColor = ;
         saver->DoRings = ;
         saver->Funky = ;
         saver->AlphaAmount = ;   
         UpdatePrefs();      
         saver->Unlock();
         break;*/
      default:
         BView::MessageReceived(msg);
         break;
   }
}

