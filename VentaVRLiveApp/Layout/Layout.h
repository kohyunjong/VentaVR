#ifndef __LAYOUT_QURAM_H__
#define __LAYOUT_QURAM_H__

#define MAX_SOURCE_CHANNELS 8

#define TAB_MODE_START 0
#define TAB_MODE_SOURCES 1
#define TAB_MODE_OUTPUT 2
#define TAB_MODE_INTERACTIVE 3
#define TAB_MODE_CONFIGURATION 4

#define ANIMATION_MODE_NONE		0x00
#define ANIMATION_MODE_ROTATE	0x01

#define ANIMATION_TIMER			3000

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Image.H>

#include "../Quram360Engine.h"
#include "../clock.h"

#include "directory.h"

class GLWin;
class Quram_Fl_Window;



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int default_w = 120;
const int default_h = 30;

const int menu_btn_w = 100;
const int menu_btn_h = 100;
const int menu_btn_gap = 10;
const int menu_output_btn_gap = 4;

const int record_btn_w = 50;
const int record_btn_h = 50;

const int subcontent_btn_w = 150;
const int subcontent_btn_h = 30;
const int subcontent_output_btn_h = 25;

const int subcontent_small_btn_w = 30;
const int subcontent_small_btn_h = 30;

const int subcontent_middle_btn_w = 55;
const int subcontent_middle_btn_h = 30;

const int screenw = Fl::w();
const int screenh = Fl::h();

const int window_x = 0;
const int window_y = 0;
const int window_h = screenh*0.8;
const int window_w = (window_h - default_h) * 2;

const int menubar_x = window_x + 0;
const int menubar_y = window_y + 0;
const int menubar_w = window_w;
const int menubar_h = 0;

const int subjectbar_x = window_x + 0;
const int subjectbar_y = menubar_y + menubar_h;
const int subjectbar_w = window_w;
const int subjectbar_h = default_h * 2;

const int subject_left_box_x = subjectbar_x;
const int subject_left_box_y = subjectbar_y;
const int subject_left_box_w = default_w;
const int subject_left_box_h = subjectbar_h;

const int time_right_box_x = subjectbar_w - default_w;
const int time_right_box_y = subjectbar_y;
const int time_right_box_w = default_w;
const int time_right_box_h = subjectbar_h;

const int mainmenu_x = window_x + 0;
const int mainmenu_y = subjectbar_y + subjectbar_h;
const int mainmenu_w = default_w;
const int mainmenu_h = window_h - mainmenu_y;

const int content_x = mainmenu_x + mainmenu_w;
const int content_y = subjectbar_y + subjectbar_h;
const int content_w = window_w - default_w;
const int content_h = window_h - content_y;

const int maincontent_x = mainmenu_x + mainmenu_w;
const int maincontent_y = subjectbar_y + subjectbar_h;
const int maincontent_w = window_w - default_w - default_w;
const int maincontent_h = maincontent_w / 2;// window_h - content_y;

const int submenu_x = window_w - default_w;
const int submenu_y = subjectbar_y + subjectbar_h;
const int submenu_w = default_w;
const int submenu_h = mainmenu_h;

const int subcontent_x = mainmenu_x + mainmenu_w;
const int subcontent_y = maincontent_y + maincontent_h;
const int subcontent_w = maincontent_w;
const int subcontent_h = window_h - subcontent_y;

const int btn_gap = 20;
const int btn_w = 200;
const int btn_h = 200;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int edit_input_window_w = 640;
const int edit_input_window_h = 610;

const int capture_config_x = 0;
const int capture_config_y = 0;
const int capture_config_w = edit_input_window_w;
const int capture_config_h = 400;

const int input_parameter_config_x = 0;
const int input_parameter_config_y = capture_config_y + capture_config_h;
const int input_parameter_config_w = edit_input_window_w;
const int input_parameter_config_h = edit_input_window_h - capture_config_h;

const int edit_input_image_w = 200;
const int edit_input_image_h = 150;

const int edit_input_text_w = 100;
const int edit_input_text_h = 30;

const int edit_input_btn_w = 150;
const int edit_input_btn_h = 30;

const int edit_input_margin_w = 10;
const int edit_input_margin_h = 10;

const int edit_input_long_margin_w = 50;
const int edit_input_long_margin_h = 50;

static char camera_index_choice_label[CAMERA_MAX][MAX_PATH];
static char image_round_button_label[CAMERA_MAX][MAX_PATH];

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int do_calibration_window_w = 410;
const int do_calibration_window_h = 130;

const int do_calibration_x = 0;
const int do_calibration_y = 0;
const int do_calibration_w = do_calibration_window_w;
const int do_calibration_h = do_calibration_window_h;

const int do_calibration_btn_w = 150;
const int do_calibration_btn_h = 30;

const int do_calibration_round_btn_w = 40;
const int do_calibration_round_btn_h = 30;

const int do_calibration_round_button_w = 410;
const int do_calibration_round_button_h = 90;

const int do_calibration_long_margin_w = 50;
const int do_calibration_long_margin_h = 50;

const int do_calibration_margin_w = 10;
const int do_calibration_margin_h = 10;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int new_configuration_window_w = 330;
const int new_configuration_window_h = 350;// 250;

const int new_configuration_x = 0;
const int new_configuration_y = 0;
const int new_configuration_w = new_configuration_window_w;
const int new_configuration_h = new_configuration_window_h;

const int new_configuration_btn_w = 150;
const int new_configuration_btn_h = 30;

const int new_configuration_long_margin_w = 50;
const int new_configuration_long_margin_h = 50;

const int new_configuration_margin_w = 10;
const int new_configuration_margin_h = 10;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int outputs_window_w = 690;
const int outputs_window_h = 770;

const int bmoutput_settings_x = 0;
const int bmoutput_settings_y = 0;
const int bmoutput_settings_w = outputs_window_w;
const int bmoutput_settings_h = 120;

const int outputs_btn_w = 150;
const int outputs_btn_h = 30;

const int outputs_margin_w = 10;
const int outputs_margin_h = 10;

const int outputs_long_margin_w = 50;
const int outputs_long_margin_h = 50;

const int output_directory_input_w = 350;
const int output_directory_input_h = 30;

const int outputs_long_btn_w = 300;
const int outputs_long_btn_h = 30;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int panorama_window_w = 330;
const int panorama_window_h = 130;

const int panorama_settings_x = 0;
const int panorama_settings_y = 0;
const int panorama_settings_w = panorama_window_w;
const int panorama_settings_h = 120;

const int panorama_btn_w = 150;
const int panorama_btn_h = 30;

const int panorama_margin_w = 10;
const int panorama_margin_h = 10;

const int panorama_long_margin_w = 50;
const int panorama_long_margin_h = 50;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int calibration_window_w = 690;
const int calibration_window_h = 500;

const int new_calibration_x = 0;
const int new_calibration_y = 0;
const int new_calibration_w = calibration_window_w;
const int new_calibration_h = 120;

const int import_calibration_x = 0;
const int import_calibration_y = new_calibration_y + new_calibration_h;
const int import_calibration_w = calibration_window_w;
const int import_calibration_h = 120;

const int setting_calibration_x = 0;
const int setting_calibration_y = import_calibration_y + import_calibration_h;
const int setting_calibration_w = calibration_window_w;
const int setting_calibration_h = calibration_window_h - new_calibration_h - import_calibration_h;

const int calibration_btn_w = 150;
const int calibration_btn_h = 30;

const int calibration_margin_w = 10;
const int calibration_margin_h = 10;

const int calibration_long_margin_w = 50;
const int calibration_long_margin_h = 50;

const int template_input_w = 350;
const int template_input_h = 30;

const int snapshot_input_w = 510;
const int snapshot_input_h = 30;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int color_setting_window_w = 320;
const int color_setting_window_h = 340;

const int color_setting_x = 0;
const int color_setting_y = 0;
const int color_setting_w = color_setting_window_w;
const int color_setting_h = color_setting_window_h;

const int round_button_w = 160;
const int round_button_h = 90;

const int color_setting_btn_w = 100;
const int color_setting_btn_h = 100;

const int color_setting_round_btn_w = 40;
const int color_setting_round_btn_h = 30;

const int color_setting_output_w = 40;
const int color_setting_output_h = 30;

const int color_setting_slider_w = 40;
const int color_setting_slider_h = 200;

const int color_setting_margin_w = 10;
const int color_setting_margin_h = 10;

const int color_setting_long_margin_w = 50;
const int color_setting_long_margin_h = 50;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/******************************/
/***DEFINE WIDGET SIZE START***/
/******************************/

const int gain_curve_manual_window_w = 525;
const int gain_curve_manual_window_h = 355;

const int gain_curve_manual_x = 0;
const int gain_curve_manual_y = 0;
const int gain_curve_manual_w = gain_curve_manual_window_w;
const int gain_curve_manual_h = gain_curve_manual_window_h;

const int gain_curve_w = 256;
const int gain_curve_h = 256;

const int gain_curve_info_w = 250;
const int gain_curve_info_h = 90;

const int gain_curve_points_w = 250;
const int gain_curve_points_h = 90;

const int gain_curve_point_w = 200;
const int gain_curve_point_h = 30;

const int point_round_button_w = 50;
const int point_round_button_h = 90;

const int channel_round_button_w = 290;
const int channel_round_button_h = 30;

const int rgb_round_button_w = 140;
const int rgb_round_button_h = 30;

const int gain_curve_manual_btn_w = 150;
const int gain_curve_manual_btn_h = 30;

const int gain_curve_manual_round_btn_w = 40;
const int gain_curve_manual_round_btn_h = 30;

const int gain_curve_manual_text_w = 20;
const int gain_curve_manual_text_h = 30;

const int gain_curve_manual_input_w = 70;
const int gain_curve_manual_input_h = 30;

const int gain_curve_manual_margin_w = 10;
const int gain_curve_manual_margin_h = 10;

const int gain_curve_manual_long_margin_w = 50;
const int gain_curve_manual_long_margin_h = 50;

/****************************/
/***DEFINE WIDGET SIZE END***/
/****************************/



/**********************************/
/***DEFINE GROBAL FUNCTION START***/
/**********************************/

int initMainLayout();
void initOutputsLayout(Quram_Fl_Window* win);
void initPanoramaLayout(Quram_Fl_Window* win);
void initCalibrationLayout(Quram_Fl_Window* win);
void initEditInputLayout(Quram_Fl_Window* win);
void initDoCalibrationLayout(Quram_Fl_Window* win);
void initNewConfigurationLayout(Quram_Fl_Window* win);
void initColorSettingLayout(Quram_Fl_Window* win);
void initGainCurveManualLayout(Quram_Fl_Window* win);
Fl_Widget* findChildByLabel(Fl_Group* parent, const char* n, const char* n2 = NULL);
void changePreviewMode(int mode, Quram_Fl_Window* win);

bool initQVREngine();
void setCallbacks();

//added get callback for App
void onCallbackDone(int msg);
/********************************/
/***DEFINE GROBAL FUNCTION END***/
/********************************/



/************************************/
/***DEFINE CALLBACK FUNCTION START***/
/************************************/

void Change_CB(Fl_Widget *w, void *);
void cb_file_select(Fl_Widget* o, void* v);
void cb_open_dialog(Fl_Widget* o, void * v);
void cb_pass(Fl_Widget* o, void* v);
void cb_quit(Fl_Widget* o, void* v);
void cb_about();
void idle(void* data);

void exit_callback(Fl_Widget* w, void* data);
void outputs_btn_callback(Fl_Widget* w, void* data);
void panorama_btn_callback(Fl_Widget* w, void* data);
void calibration_btn_callback(Fl_Widget* w, void* data);
void edit_input_choice_callback(Fl_Widget* w, void* data);
void edit_input_btn_callback(Fl_Widget* w, void* data);
void do_calibration_btn_callback(Fl_Widget* w, void* data);
void new_configuratio_btn_callback(Fl_Widget* w, void* data);
void color_setting_btn_callback(Fl_Widget* w, void* data);
void color_setting_slider_callback(Fl_Widget* w, void* data);
void gain_curve_manual_btn_callback(Fl_Widget* w, void* data);
void mainmenu_btn_callback(Fl_Widget* w, void* data);
void submenu_btn_callback(Fl_Widget* w, void* data);
void configuration_btn_callback(Fl_Widget* w, void* data);
void subcontent_btn_callback(Fl_Widget* w, void* data);

/**********************************/
/***DEFINE CALLBACK FUNCTION END***/
/**********************************/



class GLWin : public Fl_Gl_Window{
public:
	EngineManager *engineManager;
	int global_tab_mode;
	int output_draw_mode;
	bool engineReady = FALSE;//Check the QVS_Engine: 161227

	//int lastX;
	//int lastY;

	double timer;

	int interactive_animation_mode;
	float animation_speed;
	double anim_timer;

	struct RingBuf {
		float* v;
		int cur, last, K;
		RingBuf(int k) :cur(0), last(0), K(k) { v = new float[K]; }
		void push(float x) { v[cur] = x; cur = (cur + 1) % K; last++; last = last>K ? K : last; }
		float avg() { float sum = 0; for (int i = 0; i<last; i++) sum += v[i]; return sum / last; }
	};

	RingBuf* fps;
	char ttt[1024];

public:
	GLWin(int x, int y, int w, int h, const char*n = 0) :Fl_Gl_Window(x, y, w, h, n)
	{
		Configuration();
		end();

		fps = new RingBuf(60);
	}

	int handle(int e) {
		if (global_tab_mode == TAB_MODE_INTERACTIVE) {
			if (e == FL_MOUSEWHEEL) {
				//engineManager->setZoom(Fl::event_dy()*3.5);
				QVS_Engine::QVS_SetZoom(Fl::event_dy()*3.5);
				anim_timer = 0;
				return(1);
			}
		}
		else if (global_tab_mode == TAB_MODE_OUTPUT) {

		}

		return(Fl_Gl_Window::handle(e));
	}

	void Configuration()
	{
		printf("bigheadk, configuration\n");
		//glewInit();
		output_draw_mode = PANORAMA_PREVIEW_FULL;
		global_tab_mode = TAB_MODE_START;
		interactive_animation_mode = ANIMATION_MODE_NONE;
		animation_speed = 0;
		anim_timer = 0;
		engineManager = NULL;
		//engineManager = new EngineManager();
		//engineManager->Create(init_num_camera, init_input_width, init_input_height);
	}

	void resize(int X, int Y, int W, int H) {
		Fl_Gl_Window::resize(X, Y, W, H);
		redraw();
	}

	void draw()
	{
		switch (global_tab_mode)
		{
		case TAB_MODE_SOURCES:
			draw_sources();
			anim_timer = 0;
			break;
		case TAB_MODE_OUTPUT:
			draw_output();
			anim_timer = 0;
			break;
		case TAB_MODE_INTERACTIVE:
			draw_interaction();
			break;
		case TAB_MODE_START:
		case TAB_MODE_CONFIGURATION:
			anim_timer = 0;
			break;
		}
	}

	void draw_sources()
	{
		//engineManager->Drawing(w(), h());
		QVS_Engine::QVS_Drawing(w(), h());
		double diff = abs(QClock() - timer);
		fps->push(diff);
		sprintf_s(ttt, 1024, "% 5.1f FPS % 5.1f ms", 1000 / fps->avg(), fps->avg());
		timer = QClock();
	}

	void draw_interaction()
	{
		//engineManager->DrawingInteractive(w(), h());
		QVS_Engine::QVS_DrawingInteractive(w(), h());
		double diff = abs(QClock() - timer);
		if (anim_timer > ANIMATION_TIMER)
		{
			QVS_Engine::QVS_SetPan(diff / 1000.0f * animation_speed);
			if (interactive_animation_mode == ANIMATION_MODE_ROTATE)
				QVS_Engine::QVS_MulPitch(0.9);
		}
		else
		{
			anim_timer += diff;
		}
		fps->push(diff);
		sprintf_s(ttt, 1024, "% 5.1f FPS % 5.1f ms", 1000 / fps->avg(), fps->avg());
		timer = QClock();
	}

	void draw_output()
	{
		//engineManager->Stitching(w(), h(), output_draw_mode);
		QVS_Engine::QVS_Stitching(w(), h(), output_draw_mode);
		double diff = abs(QClock() - timer);
		fps->push(diff);
		sprintf_s(ttt, 1024, "% 5.1f FPS % 5.1f ms", 1000 / fps->avg(), fps->avg());
		timer = QClock();
	}
};

class Quram_Fl_Window : public Fl_Window {
public:
	GLWin* gl_main_win;
	Fl_Group *gl_main_win_group;
	Fl_Window *outputs_win;
	Fl_Window *panorama_win;
	Fl_Window *calibration_win;
	Fl_Window *edit_input_win;
	Fl_Window *do_calibration_win;
	Fl_Window *new_configuration_win;
	Fl_Window *color_setting_win;
	Fl_Window *gain_curve_manual_win;

	int output_mode;
	int select_mode;
	int mask_mode;
	int edit_mode;

	int calibration_menu;

	int fullscreen_mode;
	int trackball_mode;

	int lastX;
	int lastY;

	int push_x;
	int push_y;

	bool isDraw;
	int drawSize;

	std::string init_mode_label;

	char mStrPath[1024];
public:
	Quram_Fl_Window(int w, int h, const char* title = 0) : Fl_Window(w, h, title) {
		init();
	}

	Quram_Fl_Window(int x, int y, int w, int h, const char* title = 0) : Fl_Window(x, y, w, h, title) {
		init();
	}

	void init() {
		gl_main_win = NULL;

		output_mode = 0;
		select_mode = 0;
		mask_mode = 0;
		edit_mode = 0;
		
		calibration_menu = 0;

		fullscreen_mode = 0;
		trackball_mode = 0;

		isDraw = false;
		drawSize = 10;

		strcpy(mStrPath, ".\\");
	}

	int handle(int e) {
		// hot key
		switch (e) {
		case FL_KEYBOARD:
			if (gl_main_win->global_tab_mode == TAB_MODE_SOURCES || gl_main_win->global_tab_mode == TAB_MODE_OUTPUT || gl_main_win->global_tab_mode == TAB_MODE_INTERACTIVE) {
				int key = Fl::event_key();
				Fl_Group* content = (Fl_Group*)findChildByLabel(this, "content");
				Fl_Group* subcontent = (Fl_Group*)findChildByLabel(content, "subcontent");
				Fl_Group* submenu = (Fl_Group*)findChildByLabel(content, "submenu");
				if (key == '1') {
					gl_main_win->global_tab_mode = TAB_MODE_SOURCES;

					Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
					btn1->label("Edit Input");
					btn1->show();

					Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
					btn2->label("Calibration");
					btn2->show();

					Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
					btn4->label("Snapshot");
					btn4->show();
				}
				else if (key == '2') {
					gl_main_win->global_tab_mode = TAB_MODE_OUTPUT;

					Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
					if (output_mode == 0)
						btn1->label("Output Off");
					else if (output_mode == 1)
						btn1->label("Output On");
					btn1->show();

					Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
					btn2->label("Calibration");
					btn2->show();

					Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
					btn3->label("");
					btn3->hide();

					Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
					btn4->label("");
					btn4->hide();
				}
				else if (key == '3') {
					gl_main_win->global_tab_mode = TAB_MODE_INTERACTIVE;

					Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
					if (output_mode == 0)
						btn1->label("Output Off");
					else if (output_mode == 1)
						btn1->label("Output On");
					btn1->show();

					Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
					btn2->label("Calibration");
					btn2->show();

					Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
					btn3->label("");
					btn3->hide();

					Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
					btn4->label("");
					btn4->hide();
				}
				else if (key == '7') {
					gl_main_win->output_draw_mode = PANORAMA_PREVIEW_FULL;
				}
				else if (key == '8') {
					gl_main_win->output_draw_mode = PANORAMA_PREVIEW_UP;
				}
				else if (key == '9') {
					gl_main_win->output_draw_mode = PANORAMA_PREVIEW_MID;
				}
				else if (key == '0') {
					gl_main_win->output_draw_mode = PANORAMA_PREVIEW_DOWN;
				}
				else if (key == '[') {
					//int result = gl_main_win->engineManager->setVideoBufferOffset(-1);
					int result = QVS_Engine::QVS_SetVideoBufferOffset(-1);
					char text_video_sync[256];
					sprintf(text_video_sync, "%0.2f", -1.0f*result * (1.0f / 30.0f)); // 192000 = 1 sec
					Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
					Fl_Output* output_video_sync = (Fl_Output*)findChildByLabel(video, "sec");
					output_video_sync->value(text_video_sync);
				}
				else if (key == ']') {
					//int result = gl_main_win->engineManager->setVideoBufferOffset(1);
					int result = QVS_Engine::QVS_SetVideoBufferOffset(1);
					char text_video_sync[256];
					sprintf(text_video_sync, "%0.2f", -1.0f*result * (1.0f/30.0f)); // 192000 = 1 sec
					Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
					Fl_Output* output_video_sync = (Fl_Output*)findChildByLabel(video, "sec");
					output_video_sync->value(text_video_sync);
				}
				else if (key == 'i') {
					//gl_main_win->engineManager->setGainMode(0);
					QVS_Engine::QVS_SetGainMode(0);
				}
				else if (key == 'o') {
					//gl_main_win->engineManager->setGainMode(1);
					QVS_Engine::QVS_SetGainMode(1);
				}
				else if (key == 'p') {
					//gl_main_win->engineManager->setGainMode(2);
					QVS_Engine::QVS_SetGainMode(2);
				}
				else if (key == 'z') {
					//gl_main_win->engineManager->setMultiStreaming(0);					
					if (FL_CONTROL) {
						QVS_Engine::QVS_undo();
						//QVS_Engine::QVS_setCallback_history(onCallbackDone);
					}
					else
						QVS_Engine::QVS_SetMultiStreaming(0);
				}
				else if (key == 'y') {
					//gl_main_win->engineManager->setMultiStreaming(0);					
					if (FL_CONTROL)
						QVS_Engine::QVS_redo();
				}
				else if (key == 'x') {
					//gl_main_win->engineManager->setMultiStreaming(1);
					QVS_Engine::QVS_SetMultiStreaming(1);
				}
				else if (key == 'm')
				{
					drawSize -= 5;
					if (drawSize < 10) drawSize = 10;
				}
				else if (key == 'n')
				{
					drawSize += 5;
					if (drawSize > 100) drawSize = 100;
				}
				else if (key == 'a') {
					gl_main_win->interactive_animation_mode = ANIMATION_MODE_ROTATE;
					gl_main_win->animation_speed = 0.025;
					gl_main_win->anim_timer = 0;
					printf("Start DrawInteractive Animation\n");
				}
				else if (key == 's') {
					gl_main_win->interactive_animation_mode = ANIMATION_MODE_NONE;
					gl_main_win->animation_speed = 0;
					gl_main_win->anim_timer = 0;
					printf("Stop DrawInteractive Animation\n");
				}
				else if (key == 'v')
				{
					if (gl_main_win->interactive_animation_mode == ANIMATION_MODE_ROTATE)
					{
						gl_main_win->animation_speed += 0.005;
					}
				}
				else if (key == 'b')
				{
					if (gl_main_win->interactive_animation_mode == ANIMATION_MODE_ROTATE)
					{
						gl_main_win->animation_speed -= 0.005;
					}

					if (gl_main_win->animation_speed <= 0.0)
					{
						gl_main_win->animation_speed = 0.005;
					}
				}
				else if (key == 'f')
				{
						Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
						chooser->title("Select an output directory");
						chooser->type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
						chooser->directory(mStrPath);
						if (chooser->show() == 0) {
							sprintf(mStrPath, "%s", chooser->filename());
							
							wchar_t strUnicode[256] = { 0, };
							char strMultiByte[256] = { 0, };
							MultiByteToWideChar(CP_UTF8, 0, mStrPath, strlen(mStrPath), strUnicode, 256);
							WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);
							printf("test :: %s\n", strMultiByte);
							//FILE* fp = fopen(strMultiByte, "wt");
							int _SIZE = 10;
							char** filenamelist;
							filenamelist = (char**)malloc(sizeof(char*)* _SIZE);
							for (int i = 0; i < _SIZE; i++){
								filenamelist[i] = (char*)malloc(sizeof(char)* 1024);
							}
							getDirectoryFiles(strMultiByte, filenamelist);

							for (int i = 0; i < _SIZE; i++){
								QVS_Engine::QVS_setInputImagePath(i, filenamelist[i]);
								QVS_Engine::QVS_setCaptureDeviceIndex(i, QVS_Engine::QVS_getCaptureDeviceSize() - 1);
								QVS_Engine::QVS_SaveParam();
							}
							for (int i = 0; i < _SIZE; i++){
								free(filenamelist[i]);
							}
							free(filenamelist);
						}
				}
			}
		}

		// full screen mode
		switch (e) {
		case FL_KEYBOARD:
			int key = Fl::event_key();
			switch (key) {
			case FL_Home:
				break;
			case FL_Enter:
				if (gl_main_win->global_tab_mode != TAB_MODE_START && gl_main_win->global_tab_mode != TAB_MODE_CONFIGURATION) {
					if (Fl::event_state() & FL_ALT) {
						if (fullscreen_mode == 0) {
							ShowCursor(false);
							fullscreen();

							int x;
							int y;
							int w;
							int h;
							Fl::screen_xywh(x, y, w, h);

							gl_main_win->resize(0, 0, w, h);

							fullscreen_mode = 1;
							trackball_mode = 1;

							lastX = Fl::event_x();
							lastY = Fl::event_y();
						}
					}
				}

				return 1;
			case FL_Escape:
				if (gl_main_win->global_tab_mode != TAB_MODE_START && gl_main_win->global_tab_mode != TAB_MODE_CONFIGURATION) {
					if (fullscreen_mode == 1) {
						ShowCursor(true);
						fullscreen_off();

						Fl_Group* content = (Fl_Group*)(this->child(3));
						Fl_Group* maincontent = (Fl_Group*)(content->child(0));

						int x = maincontent->x() + 10;
						int y = maincontent->y() + 10;
						int w = maincontent->w() - 20;
						int h = maincontent->h() - 20;
						gl_main_win->resize(x, y, w, h);

						fullscreen_mode = 0;
						trackball_mode = 0;
						
					}
				}

				return 1;
			case FL_CTRL:
				printf("00key=%d\n", key);
				if (Fl::event_state() & key == 'y')
				{
					printf("111key=%d\n", key);
					if (Fl::event_state() & FL_CONTROL)
					{
						printf("222key=%d\n", key);
						QVS_Engine::QVS_redo();
					}
				}
				return 1;
			}
		}

		// user interaction
		if (gl_main_win->global_tab_mode == TAB_MODE_INTERACTIVE) {
			/*
			switch (e) {
			case FL_KEYBOARD:
				int key = Fl::event_key();
				if (fullscreen_mode == 1) {
					if (key == 't') {
						if (trackball_mode == 0) {
							lastX = Fl::event_x();
							lastY = Fl::event_y();

							trackball_mode = 1;
							//gl_main_win->take_focus();
							//ShowCursor(false);
						}
						else if (trackball_mode == 1) {
							trackball_mode = 0;
							//ShowCursor(true);
						}

						return 1;
					}
				}

				gl_main_win->engineManager->getKey(key);
				return(1);
			}
			*/
			if (trackball_mode == 0) {
				if (e == FL_PUSH) {
					lastX = Fl::event_x();
					lastY = Fl::event_y();
					//return(1);
				}

				if (e == FL_DRAG) {
					float x = Fl::event_x(), y = Fl::event_y();
					//gl_main_win->engineManager->setPan(-(x - lastX) / (float)w());
					//gl_main_win->engineManager->setPitch(-(y - lastY) / (float)h());
					QVS_Engine::QVS_SetPan(-(x - lastX) / (float)w());
					QVS_Engine::QVS_SetPitch(-(y - lastY) / (float)h());
					gl_main_win->anim_timer = 0;
					lastX = x;
					lastY = y;
					return(1);
				}

				if (e == FL_MOUSEWHEEL) {
					//gl_main_win->engineManager->setZoom(Fl::event_dy() * 3.5);
					QVS_Engine::QVS_SetZoom(Fl::event_dy() * 3.5);
					gl_main_win->anim_timer = 0;
					return(1);
				}
			}
			else if (trackball_mode == 1) {
				if (e == FL_MOVE) {
					float x = Fl::event_x(), y = Fl::event_y();
					//gl_main_win->engineManager->setPan((x - lastX) / (float)w()*0.5f);
					//gl_main_win->engineManager->setPitch((y - lastY) / (float)h()*0.5f);
					QVS_Engine::QVS_SetPan((x - lastX) / (float)w()*0.5f);
					QVS_Engine::QVS_SetPitch((y - lastY) / (float)h()*0.5f);
					gl_main_win->anim_timer = 0;

					SetCursorPos(Fl::w() / 2, Fl::h() / 2);
					lastX = Fl::w() / 2;
					lastY = Fl::h() / 2;
					return(1);
				}

				if (e == FL_MOUSEWHEEL) {
					//gl_main_win->engineManager->setZoom(Fl::event_dy() * 3.5);
					QVS_Engine::QVS_SetZoom(Fl::event_dy() * 3.5);
					gl_main_win->anim_timer = 0;
					return(1);
				}

				return 1;
			}
		}
		else if (gl_main_win->global_tab_mode == TAB_MODE_OUTPUT) {
			

			if (select_mode == 1) {
				if (e == FL_LEFT_MOUSE) {
					int m_isMLSelectClicked;
					QVS_Engine::QVS_selectROI((Fl::event_x() - gl_main_win->x()) * QVS_Engine::QVS_GetOutputWidth() / gl_main_win->w(), (Fl::event_y() - gl_main_win->y()) * QVS_Engine::QVS_GetOutputHeight() / gl_main_win->h(), m_isMLSelectClicked);
					printf("JW hanlde QVS_selectROI: (QVS_Engine::QVS_GetOutputWidth(),QVS_Engine::QVS_GetOutputHeight()) = (%d, %d)\n", QVS_Engine::QVS_GetOutputWidth(), QVS_Engine::QVS_GetOutputHeight());
				}
			}
			else if (mask_mode == 1) {
				if (e == FL_MOUSEWHEEL) {
					drawSize -= 5 * Fl::event_dy();
					if (drawSize < 10)
						drawSize = 10;
					else if (drawSize > 100)
						drawSize = 100;
					return(1);
				}

				if (isDraw == true) {
					int move_x = Fl::event_x() - gl_main_win->x();
					int move_y = Fl::event_y() - gl_main_win->y();

					if (move_x < 0)
						move_x = 0;
					else if (move_x > gl_main_win->w() - 1)
						move_x = gl_main_win->w() - 1;
					if (move_y < 0)
						move_y = 0;
					else if (move_y > gl_main_win->h() - 1)
						move_y = gl_main_win->h() - 1;

					int drawMode = Fl::event_button() == FL_LEFT_MOUSE ? 1 : 0;

					QVS_Engine::QVS_drawROI(move_x * QVS_Engine::QVS_GetOutputWidth() / gl_main_win->w(), move_y * QVS_Engine::QVS_GetOutputHeight() / gl_main_win->h(), drawSize, drawMode);
					printf("JW hanlde QVS_drawROI: (QVS_Engine::QVS_GetOutputWidth(),QVS_Engine::QVS_GetOutputHeight()) = (%d, %d)\n", QVS_Engine::QVS_GetOutputWidth(), QVS_Engine::QVS_GetOutputHeight());
				}

				if (e == FL_PUSH) {
					push_x = Fl::event_x();
					push_y = Fl::event_y();

					if (push_x > gl_main_win->x() && push_x < gl_main_win->x() + gl_main_win->w() &&
						push_y > gl_main_win->y() && push_y < gl_main_win->y() + gl_main_win->h())
					{
						push_x -= gl_main_win->x();
						push_y -= gl_main_win->y();

						isDraw = true;
					}
				}
				else if (e == FL_RELEASE) {
					isDraw = false;

					QVS_Engine::QVS_drawROIFinish();
				}
			}
			else if (edit_mode == 1){
				if (e == FL_PUSH) {
					lastX = Fl::event_x();
					lastY = Fl::event_y();
					//return(1);
				}

				if (e == FL_DRAG) {
					float x = Fl::event_x();
					float y = Fl::event_y();
					int isRoll, isReverseX, isReverseY;

					if (Fl::event_button() == FL_LEFT_MOUSE)
						isRoll = 0;
					else if (Fl::event_button() == FL_RIGHT_MOUSE) {
						isRoll = 1;
						if (w() / 2 > lastX)
							isReverseX = 0;
						else
							isReverseX = 1;

						if (h() / 2 > lastY)
							isReverseY = 0;
						else
							isReverseY = 1;
					}

					QVS_Engine::QVS_moveOutput((x - lastX) / (float)w(), (y - lastY) / (float)h(), isRoll, isReverseX, isReverseY);
					gl_main_win->anim_timer = 0;
					lastX = x;
					lastY = y;
					return(1);
				}
			}
		}

		return Fl_Window::handle(e);
	}

	void resize(int x, int y, int w, int h) {
		Fl_Window::resize(x, y, w, h);

		//do something after resize
		if (gl_main_win->global_tab_mode != TAB_MODE_START && gl_main_win->global_tab_mode != TAB_MODE_CONFIGURATION) {
			int gl_main_win_x = gl_main_win_group->x() + 10;
			int gl_main_win_y = gl_main_win_group->y() + 10;
			int gl_main_win_w = gl_main_win_group->w() - 20;
			int gl_main_win_h = gl_main_win_group->h() - 20;
			gl_main_win->resize(gl_main_win_x, gl_main_win_y, gl_main_win_w, gl_main_win_h);
			//gl_main_win->redraw();
		}
	}

	static void timer_cb(void *data) {
		Quram_Fl_Window *win = (Quram_Fl_Window*)data;
		Fl_Group *subjectbar = (Fl_Group*)win->child(2);
		Fl_Box *box_time_fps = (Fl_Box*)subjectbar->child(3);
		if (win->gl_main_win->global_tab_mode != TAB_MODE_START)
			box_time_fps->label(win->gl_main_win->ttt);
		Fl::repeat_timeout(1, timer_cb, win);
	}
};

#endif // __LAYOUT_QURAM_H__