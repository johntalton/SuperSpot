#ifndef MY_CONFIG
#define MY_CONFIG
#include <View.h>

class MinimalSaver;
class MySSConfigPanel : public BView {
public:
   MySSConfigPanel(BRect frame, MinimalSaver *saver);
   void	AttachedToWindow(void);
   void	DetachedFromWindow(void);
   void UpdatePrefs(void);
   void	MessageReceived(BMessage *message);
   void AboutRequested();
	
private:
   MinimalSaver *saver;
   BSlider *size_slider;
   BCheckBox *randomize;
   BCheckBox *blank;
   BCheckBox *randcolor;
   BColorControl *Cpicker;
   BCheckBox *rings;
   BCheckBox *funk;
   BSlider *alpha_slider;
   BMenu *Fmenu;
   BMenuItem *item;
   uint32 window_flags;
};
#endif