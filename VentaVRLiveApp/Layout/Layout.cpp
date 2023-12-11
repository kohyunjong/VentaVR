#include "Layout.h"

Fl_Widget* findChildByLabel(Fl_Group* parent, const char* n, const char* n2) {
	for (int i = 0; i < parent->children(); i++) {
		if (parent->child(i)->label() == n) {
			return parent->child(i);
		}
	}

	if (n2 != NULL) {
		for (int i = 0; i < parent->children(); i++) {
			if (parent->child(i)->label() == n2) {
				return parent->child(i);
			}
		}
	}

	return NULL;
}

void changePreviewMode(int mode, Quram_Fl_Window* win) {
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* submenu = (Fl_Group*)findChildByLabel(content, "submenu");
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	win->select_mode = 0;
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	win->mask_mode = 0;
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);

	Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
	btn1->activate();

	Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
	btn2->activate();

	Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
	btn3->activate();

	Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
	btn4->activate();

	if (mode == TAB_MODE_SOURCES) {
		win->gl_main_win->global_tab_mode = TAB_MODE_SOURCES;

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
	else if (mode == TAB_MODE_OUTPUT) {
		win->gl_main_win->global_tab_mode = TAB_MODE_OUTPUT;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Edit Overlay");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("Calibration");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		if (win->edit_mode == 0)
			btn3->label("Edit Off");
		else if (win->edit_mode == 1)
			btn3->label("Edit On");
		btn3->show();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		if (win->edit_mode == 0) {
			btn4->label("Pano Save");
			btn4->show();
		}
		else if (win->edit_mode == 1) {
			btn4->label("Straighten");
			btn4->show();
		}		
	}
	else if (mode == TAB_MODE_INTERACTIVE) {
		win->gl_main_win->global_tab_mode = TAB_MODE_INTERACTIVE;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Edit Overlay");
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
	else if (mode == TAB_MODE_CONFIGURATION) {
		win->gl_main_win->global_tab_mode = TAB_MODE_CONFIGURATION;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("");
		btn1->hide();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("");
		btn2->hide();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("");
		btn3->hide();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("");
		btn4->hide();
	}
}

void Change_CB(Fl_Widget *w, void *) {
	Fl_Menu_Bar *menu = (Fl_Menu_Bar*)w;
	Fl_Menu_Item *p;
	// Change submenu name
	p = (Fl_Menu_Item*)menu->find_item("Edit/Submenu");
	if (p) p->label("New Submenu Name");
	// Change item name
	p = (Fl_Menu_Item*)menu->find_item("Edit/New Submenu Name/Aaa");
	if (p) p->label("New Aaa Name");
}

void cb_file_select(Fl_Widget* o, void* v) {
	
}

void cb_open_dialog(Fl_Widget* o, void * v){
	//dlg->show();
}

void cb_pass(Fl_Widget* o, void* v){
	fl_message((char*)v);
}

void cb_quit(Fl_Widget* o, void* v){
	//dlg->hide();
	//window->hide();
	exit(0);
}

void cb_about(){
	fl_message("This is about .");
}


void idle(void* data) {
	GLWin* gl_main_win = (GLWin*)data;
	if (gl_main_win)
	{
		//if (gl_main_win->engineManager)
		if(gl_main_win->engineReady)
		{
			Quram_Fl_Window* win = (Quram_Fl_Window*)gl_main_win->parent();
			gl_main_win->redraw();

			//if (gl_main_win->engineManager->mGainCurveData[gl_main_win->engineManager->mGainChannelIndex])
			if (QVS_Engine::QVS_GetGainCurveData())
			{
				Fl_Group* gain_curve_manual = (Fl_Group*)findChildByLabel(win->gain_curve_manual_win, "gain_curve_manual");
				Fl_Group* gain_curve = (Fl_Group*)findChildByLabel(gain_curve_manual, "gain_curve");
				Fl_Box* gain_curve_graph = (Fl_Box*)findChildByLabel(gain_curve, "Gain Curve Graph");

				//Fl_RGB_Image *gain_curve_image = new Fl_RGB_Image((unsigned char*)gl_main_win->engineManager->mGainCurveData[gl_main_win->engineManager->mGainChannelIndex], gain_curve_w, gain_curve_h);
				Fl_RGB_Image *gain_curve_image = new Fl_RGB_Image((unsigned char*)QVS_Engine::QVS_GetGainCurveData(), gain_curve_w, gain_curve_h);
				gain_curve_graph->image(gain_curve_image);

				gain_curve->redraw();

				//gl_main_win->engineManager->mGainCurveData[gl_main_win->engineManager->mGainChannelIndex] = NULL;
				QVS_Engine::QVS_SetGainCurveData(NULL);
			}
		}
	}
}

void exit_callback(Fl_Widget* w, void* data) {
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	printf("bigheadk, exit_callback\n");
	//if (manager && manager->IsNeedSave()) {
	if (QVS_Engine::QVS_IsNeedSave()) {
		int ret = fl_choice("Do you want to save changes to the modified files?", "Cancel", "Yes", "No");
		if (ret == 0) {
			// Cancel
			// Do nothing...
		}
		else if (ret == 1) {
			// Yes
			// Save Status
			//manager->SaveParam(1);
			QVS_Engine::QVS_SaveParam(1);
			QVS_Engine::QVS_destroy();
			
			exit(1);
			
		}
		else if (ret == 2) {
			// No
			QVS_Engine::QVS_destroy();
			exit(1);
			
		}
	}
	else {
		QVS_Engine::QVS_destroy();
		exit(1);
		
	}
}

void outputs_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// bmoutput menu

	// common menu
	if (btn->label() == "Save") {
		QVS_Engine::Configuration *config = new QVS_Engine::Configuration();

		int bmoutput_device;
		int bmoutput_display;
		{
			Fl_Group* bmoutput_settings = (Fl_Group*)findChildByLabel(btn->parent(), "bmoutput_settings");

			Fl_Choice* device_choice = (Fl_Choice*)findChildByLabel(bmoutput_settings, "device_choice");
			Fl_Choice* display_choice = (Fl_Choice*)findChildByLabel(bmoutput_settings, "display_choice");

			bmoutput_device = device_choice->value();
			bmoutput_display = display_choice->value();
		}

		config->bmoutput_device = bmoutput_device;
		config->bmoutput_display = bmoutput_display;

		//manager->setOutputConfiguration(hdd_width, hdd_height, hdd_path, hdd_bitrate_mode, hdd_bitrate, hdd_gop, hdd_audio_bitrate, hdd_color_space, hdd_preset, hdd_fps, hdd_codec,
		//	rtmp_width, rtmp_height, rtmp_path, rtmp_bitrate_mode, rtmp_bitrate, rtmp_gop, rtmp_audio_bitrate, rtmp_color_space, rtmp_preset, rtmp_fps, rtmp_codec, bmoutput_device, bmoutput_display);
		//manager->SaveParam();

		int rtn = QVS_Engine::QVS_SetOutputConfiguration(config);
		QVS_Engine::QVS_SaveParam();

		win->outputs_win->hide();
	}
}

void panorama_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	int panoramaSize;

	Fl_Group* panorama_settings = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "panorama_settings");
	Fl_Choice* size_choice = (Fl_Choice*)findChildByLabel(panorama_settings, "size_choice");
	panoramaSize = size_choice->value();

	//manager->setPanoramaSize(panoramaSize);
	//manager->SaveParam();
	QVS_Engine::QVS_SetPanoramaSize(panoramaSize);
	QVS_Engine::QVS_SaveParam();

	win->panorama_win->hide();
}

void calibration_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* maincontent = (Fl_Group*)findChildByLabel(content, "maincontent");
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
	GLWin* gl_main_win = win->gl_main_win;

	// import menu
	if (btn->parent()->label() == "import_calibration") {
		if (btn->label() == "Browse...") {
			char path[256];
			Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
			chooser->title("Select calibration template");
			chooser->filter("PTGui File\t*.pts");
			chooser->directory(".\\");
			if (chooser->show() == 0) {
				sprintf(path, "%s", chooser->filename());

				Fl_Input* input = (Fl_Input*)findChildByLabel(btn->parent(), "template_input");
				input->value(path);
			}
		}
		else if (btn->label() == "Apply") {
			gl_main_win->global_tab_mode = TAB_MODE_OUTPUT;

			Fl_Input *input = (Fl_Input*)findChildByLabel(btn->parent(), "template_input");
			//if (manager->ParsingPTS(input->value()) == 0) {
			if (QVS_Engine::QVS_ParsingPTS(input->value()) == 0) {
				fl_message("Import Template Fail!");
				return;
			}
			//manager->SaveParam();
			QVS_Engine::QVS_SaveParam();

			Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
			Fl_Button* btn_panorama = (Fl_Button*)findChildByLabel(mainmenu, "Panorama");
			mainmenu_btn_callback(btn_panorama, win);

			win->calibration_win->hide();
		}
	}

	// setting menu
	if (btn->parent()->label() == "setting_calibration") {
		if (btn->label() == "Browse...") {
			char path[256];
			Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
			chooser->title("Select snapshot directory");
			chooser->type(Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY);
			chooser->directory(".\\");
			if (chooser->show() == 0) {
				sprintf(path, "%s", chooser->filename());

				Fl_Input* input = (Fl_Input*)findChildByLabel(btn->parent(), "snapshot_input");
				input->value(path);

				//manager->setSnapshotPath(input->value());
				//manager->SaveParam();
				QVS_Engine::QVS_SetSnapshotPath(input->value());
				QVS_Engine::QVS_SaveParam();
			}
		}
	}
}

void edit_input_choice_callback(Fl_Widget* w, void* data) {
	Fl_Choice* choice = (Fl_Choice*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	char choice_label[256];
	strcpy(choice_label, choice->label());
	char *temp = strtok(choice_label, " ");
	char *camera_index;

	while (temp != NULL) {
		camera_index = temp;
		temp = strtok(NULL, " ");
	}

	int selCaptureDeviceIndex = choice->value();
	
	// Select "No Device"
	if (selCaptureDeviceIndex == 0) {
		QVS_Engine::QVS_destroyVideoCapture(camera_index[0] + (0 - '0'));
	}
	// Select "..."
	else if (selCaptureDeviceIndex == QVS_Engine::QVS_getCaptureDeviceSize() + 1) {
		Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
		chooser->title("Select input image file");
		chooser->filter("JPEG\t*.{jpg,jpeg}");
		chooser->directory(".\\");
		if (chooser->show() == 0) {
			QVS_Engine::QVS_setInputImagePath(camera_index[0] + (0 - '0'), (char*)chooser->filename());
			QVS_Engine::QVS_setCaptureDeviceIndex(camera_index[0] + (0 - '0'), QVS_Engine::QVS_getCaptureDeviceSize() - 1);
			QVS_Engine::QVS_SaveParam();
		}
		choice->value(QVS_Engine::QVS_getCaptureDeviceSize());
	}
	else {
		QVS_Engine::QVS_setCaptureDeviceIndex(camera_index[0] + (0 - '0'), selCaptureDeviceIndex - 1);
		QVS_Engine::QVS_SaveParam();
		//choice->value
		
		Fl_Group* capture_config = (Fl_Group*)findChildByLabel(choice->parent()->parent(), "capture_config");
		for (int i = 0; i < CAMERA_MAX; i++) {
			Fl_Choice* camera_index_choice = (Fl_Choice*)findChildByLabel(capture_config, camera_index_choice_label[i]);
			camera_index_choice->clear();
			camera_index_choice->add("No Device");
			for (int j = 0; j < QVS_Engine::QVS_getCaptureDeviceSize(); j++) {
				camera_index_choice->add(QVS_Engine::QVS_getCaptureDeviceName(j));
			}
			camera_index_choice->add("...Image");
			camera_index_choice->value(QVS_Engine::QVS_getCaptureDeviceIndex(i) + 1);
		}
		
	}
}

void edit_input_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// common menu
	if (btn->label() == "Save") {
		float fps;
		int width;
		int height;
		int lens_type;
		float fov;

		Fl_Group* input_parameter_config = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "input_parameter_config");

		Fl_Input* fps_input = (Fl_Input*)findChildByLabel(input_parameter_config, "fps_input");
		Fl_Input* width_input = (Fl_Input*)findChildByLabel(input_parameter_config, "width_input");
		Fl_Input* height_input = (Fl_Input*)findChildByLabel(input_parameter_config, "height_input");
		Fl_Input* fov_input = (Fl_Input*)findChildByLabel(input_parameter_config, "fov_input");
		Fl_Choice* choice_lens = (Fl_Choice*)findChildByLabel(input_parameter_config, "choice_lens");


		sscanf(fps_input->value(), "%f", &fps);
		sscanf(width_input->value(), "%d", &width);
		sscanf(height_input->value(), "%d", &height);
		sscanf(fov_input->value(), "%f", &fov);
		lens_type = QVS_Engine::QVS_saveLensType(choice_lens->value());

		//manager->setInputConfiguration(fps, width, height);
		//manager->SaveParam();
		QVS_Engine::QVS_SetInputConfiguration(fps, width, height, lens_type, fov);
		QVS_Engine::QVS_SaveParam();

		win->edit_input_win->hide();
	}
}

void do_calibration_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// common menu
	if (btn->label() == "Apply") {
		Fl_Group* do_calibration = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "do_calibration");
		Fl_Group* round_button = (Fl_Group*)findChildByLabel(do_calibration, "round_button");
		for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++) {
			Fl_Round_Button* image_round_button = (Fl_Round_Button*)findChildByLabel(round_button, image_round_button_label[i]);
			//manager->mCalibrationIndex[i] = image_round_button->value();
		}
		//QVS_Engine::QVS_setCallback_quramCalibMode(onCallbackDone);
		QVS_Engine::QVS_SetQuramCalibMode(1);
	}
}

void new_configuratio_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* subcontent = (Fl_Group*)findChildByLabel(content, "subcontent");

	// common menu
	if (btn->label() == "Save") {

		Fl_Group* new_configuration = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "new_configuration");

		Fl_Choice* capture_type_choice = (Fl_Choice*)findChildByLabel(new_configuration, "capture_type_choice");
		Fl_Input* number_of_camera_input = (Fl_Input*)findChildByLabel(new_configuration, "number_of_camera_input");
		Fl_Input* width_input = (Fl_Input*)findChildByLabel(new_configuration, "width_input");
		Fl_Input* height_input = (Fl_Input*)findChildByLabel(new_configuration, "height_input");
		Fl_Input* width_output = (Fl_Input*)findChildByLabel(new_configuration, "output_width_input");
		Fl_Input* height_output = (Fl_Input*)findChildByLabel(new_configuration, "output_height_input");

		int number_of_camera;
		int width;
		int height;
		int output_width;
		int output_height;

		sscanf(number_of_camera_input->value(), "%d", &number_of_camera);
		sscanf(width_input->value(), "%d", &width);
		sscanf(height_input->value(), "%d", &height);
		sscanf(width_output->value(), "%d", &output_width);
		sscanf(height_output->value(), "%d", &output_height);

		if (number_of_camera < 1 || number_of_camera > 8) {
			fl_message("Number of cameras must be 1~8.");
			return;
		}
		else if (width < 10 || height < 10 || width > 5000 || height > 5000) {
			fl_message("Input size is invalid.");
			return;
		}

		win->new_configuration_win->hide();

		// start menu
		if (win->init_mode_label == "New\n\nConfiguration") {
			const char* msg = fl_input("New Configuration");
			if (msg != NULL) {
				Fl_Group* subjectbar = (Fl_Group*)findChildByLabel(win, "subjectbar");
				subjectbar->child(2)->label(msg);

				char path[MAX_PATH];
				sprintf(path, ".\\%s.q360", msg);
				//FILE* fp = fopen(path, "w");
				//if (fp) fclose(fp);

				win->gl_main_win->global_tab_mode = TAB_MODE_START;
				//win->gl_main_win->engineManager = new EngineManager();
				//win->gl_main_win->engineManager->Create(capture_type, number_of_camera, channelIndex, width, height);
				QVS_Engine::QVS_Create(number_of_camera, width, height, output_width, output_height, QVS_Engine::StitchingMode_Stereo);
				win->gl_main_win->engineReady = TRUE;
				setCallbacks();

				//EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
				//manager->setSavePath(path);
				//manager->SaveParam();
				QVS_Engine::QVS_SetSavePath(path);
				QVS_Engine::QVS_SaveParam();

				Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
				Fl_Widget* btn_Sources = findChildByLabel(mainmenu, "Sources");
				Fl_Widget* btn_Panorama = findChildByLabel(mainmenu, "Panorama");
				Fl_Widget* btn_Interactive = findChildByLabel(mainmenu, "Interactive");
				Fl_Widget* btn_Configuration = findChildByLabel(mainmenu, "Configuration");

				btn_Sources->show();
				btn_Panorama->show();
				btn_Interactive->show();
				btn_Configuration->show();

				mainmenu_btn_callback(btn_Sources, win);
			}
		}
		/*
		else if (win->init_mode_label == "Load\n\nConfiguration") {
			char path[256];
			Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
			chooser->title("Open 360 Camera configuration");
			chooser->filter("SKT 360 Video File\t*.q360");
			chooser->directory(".\\");
			if (chooser->show() == 0) {
				sprintf(path, "%s", chooser->filename());

				win->gl_main_win->global_tab_mode = TAB_MODE_START;
				win->gl_main_win->engineManager = new EngineManager();
				win->gl_main_win->engineManager->Create(capture_type, number_of_camera, width, height);

				EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
				if (manager->LoadParam(path) == 0) {
					fl_message("Load Configuration Fail!");
					return;
				}

				char *temp_w = strtok((char*)chooser->filename(), "\\");
				char *subject_name;

				while (temp_w != NULL) {
					subject_name = temp_w;
					temp_w = strtok(NULL, "\\");
				}

				subject_name = strtok(subject_name, ".");

				Fl_Group* subjectbar = (Fl_Group*)findChildByLabel(win, "subjectbar");
				subjectbar->child(2)->label(subject_name);

				Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
				Fl_Widget* btn_Sources = findChildByLabel(mainmenu, "Sources");
				Fl_Widget* btn_Panorama = findChildByLabel(mainmenu, "Panorama");
				Fl_Widget* btn_Interactive = findChildByLabel(mainmenu, "Interactive");
				Fl_Widget* btn_Configuration = findChildByLabel(mainmenu, "Configuration");

				btn_Sources->show();
				btn_Panorama->show();
				btn_Interactive->show();
				btn_Configuration->show();

				Fl_Button* btn_exposure_mode = (Fl_Button*)findChildByLabel(subcontent, "Normal Blending", "Multiband Blending");
				if (manager->mBlendingMode == GRADATION_BLEND)
					btn_exposure_mode->label("Normal Blending");
				else if (manager->mBlendingMode == LAPLACIAN_BLEND)
					btn_exposure_mode->label("Multiband Blending");

				Fl_Button* btn_seam_mode = (Fl_Button*)findChildByLabel(subcontent, "Seam Image", "Seam Dist");
				if (manager->mSeamMode == FIND_SEAM_FROM_IMAGE)
					btn_seam_mode->label("Seam Image");
				else if (manager->mSeamMode == FIND_SEAM_FROM_DIST)
					btn_seam_mode->label("Seam Dist");

				Fl_Choice* choice_gain = (Fl_Choice*)findChildByLabel(subcontent, "choice_gain");
				choice_gain->value(manager->mGainMode);

				Fl_Button* btn_nadir_mode = (Fl_Button*)findChildByLabel(subcontent, "Nadir Off", "Nadir On");
				if (manager->mNadirMode == NADIR_LOG_OFF)
					btn_nadir_mode->label("Nadir Off");
				else if (manager->mNadirMode == NADIR_LOG_ON)
					btn_nadir_mode->label("Nadir On");

				Fl_Button* btn_nadir_position = (Fl_Button*)findChildByLabel(subcontent, "Nadir BOTTOM", "Nadir TOP");
				if (manager->mNadirPosition == NADIR_BOTTOM)
					btn_nadir_position->label("Nadir BOTTOM");
				else if (manager->mNadirPosition == NADIR_TOP)
					btn_nadir_position->label("Nadir TOP");

				Fl_Counter* counter_size = (Fl_Counter*)findChildByLabel(subcontent, "counter_size");
				counter_size->value(manager->mNadirSize);

				mainmenu_btn_callback(btn_Sources, win);
			}
		}
		*/
	}
}

void color_setting_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// button
	if (btn->label() == "Sources") {
		mainmenu_btn_callback(w, data);
	}
	else if (btn->label() == "Panorama") {
		mainmenu_btn_callback(w, data);
	}
	else if (btn->label() == "Interactive") {
		mainmenu_btn_callback(w, data);
	}

	// round button
	if (btn->label() == "1" || btn->label() == "2" || btn->label() == "3" || btn->label() == "4" || btn->label() == "5" || btn->label() == "6") {
		int index;
		sscanf(btn->label(), "%d", &index);
		//manager->mRGBIndex = index - 1;
		QVS_Engine::QVS_SetRGBIndex(index - 1);

		Fl_Value_Slider* slider_red = (Fl_Value_Slider*)findChildByLabel(btn->parent()->parent(), "RED");
		Fl_Value_Slider* slider_green = (Fl_Value_Slider*)findChildByLabel(btn->parent()->parent(), "GREEN");
		Fl_Value_Slider* slider_blue = (Fl_Value_Slider*)findChildByLabel(btn->parent()->parent(), "BLUE");

		//slider_red->value(manager->mRed[manager->mRGBIndex]);
		//slider_green->value(manager->mGreen[manager->mRGBIndex]);
		//slider_blue->value(manager->mBlue[manager->mRGBIndex]);
		slider_red->value(QVS_Engine::QVS_GetRed());
		slider_green->value(QVS_Engine::QVS_GetGreen());
		slider_blue->value(QVS_Engine::QVS_GetBlue());
		
	}
}

void color_setting_slider_callback(Fl_Widget* w, void* data) {
	Fl_Value_Slider* slider = (Fl_Value_Slider*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// slider
	if (slider->label() == "RED" || slider->label() == "GREEN" || slider->label() == "BLUE") {
		int red;
		int green;
		int blue;

		Fl_Value_Slider* slider_red = (Fl_Value_Slider*)findChildByLabel(slider->parent(), "RED");
		Fl_Value_Slider* slider_green = (Fl_Value_Slider*)findChildByLabel(slider->parent(), "GREEN");
		Fl_Value_Slider* slider_blue = (Fl_Value_Slider*)findChildByLabel(slider->parent(), "BLUE");

		red = slider_red->value();
		if (red == 0)
		{
			slider_red->value(0.00001);
		}

		green = slider_green->value();
		if (green == 0)
		{
			slider_green->value(0.00001);
		}

		blue = slider_blue->value();
		if (blue == 0)
		{
			slider_blue->value(0.00001);
		}

		//manager->setManualRGB(manager->mRGBIndex, red, green, blue);
		QVS_Engine::QVS_SetManualRGB(QVS_Engine::QVS_GetRGBIndex(), red, green, blue);
	}
}

void gain_curve_manual_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	//QVS_Engine::QVS_setCallback_doManualGainMode(onCallbackDone2);

	// button
	if (btn->label() == "Apply") {
		Fl_Group* gain_curve_points = (Fl_Group*)findChildByLabel(btn->parent(), "gain_curve_points");
		Fl_Group* p1 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point1");
		Fl_Float_Input* p1_x = (Fl_Float_Input*)findChildByLabel(p1, "point_x");
		Fl_Float_Input* p1_y = (Fl_Float_Input*)findChildByLabel(p1, "point_y");
		Fl_Group* p2 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point2");
		Fl_Float_Input* p2_x = (Fl_Float_Input*)findChildByLabel(p2, "point_x");
		Fl_Float_Input* p2_y = (Fl_Float_Input*)findChildByLabel(p2, "point_y");
		Fl_Group* p3 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point3");
		Fl_Float_Input* p3_x = (Fl_Float_Input*)findChildByLabel(p3, "point_x");
		Fl_Float_Input* p3_y = (Fl_Float_Input*)findChildByLabel(p3, "point_y");

		double point_value;
		
		point_value = -1.f;
		sscanf(p1_x->value(), "%lf", &point_value);
		if (point_value < 0) {
			p1_x->value("0");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = 0.0f;
			QVS_Engine::QVS_SetGainPointX1(0.0f);
		}
		else if (point_value > 255) {
			p1_x->value("255");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = 255.0f;
			QVS_Engine::QVS_SetGainPointX1(255.0f);
		}
		else {
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = point_value;
			QVS_Engine::QVS_SetGainPointX1(point_value);
		}
		point_value = -1.f;
		sscanf(p1_y->value(), "%lf", &point_value);
		if (point_value < 0) {
			p1_y->value("0");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = 0.0f;
			QVS_Engine::QVS_SetGainPointY1(0.0f);
		}
		else if (point_value > 255) {
			p1_y->value("255");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = 255.0f;
			QVS_Engine::QVS_SetGainPointY1(255.0f);
		}
		else {
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][0] = point_value;
			QVS_Engine::QVS_SetGainPointY1(point_value);
		}

		point_value = -1.f;
		sscanf(p2_x->value(), "%lf", &point_value);
		if (point_value < 0) {
			p2_x->value("0");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = 0.0f;
			QVS_Engine::QVS_SetGainPointX2(0.0f);
		}
		else if (point_value > 255) {
			p2_x->value("255");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = 255.0f;
			QVS_Engine::QVS_SetGainPointX2(255.0f);
		}
		else {
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = point_value;
			QVS_Engine::QVS_SetGainPointX2(point_value);
		}
		point_value = -1.f;
		sscanf(p2_y->value(), "%lf", &point_value);
		if (point_value < 0) {
			p2_y->value("0");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = 0.0f;
			QVS_Engine::QVS_SetGainPointY2(0.0f);
		}
		else if (point_value > 255) {
			p2_y->value("255");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = 255.0f;
			QVS_Engine::QVS_SetGainPointY2(255.0f);
		}
		else {
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][1] = point_value;
			QVS_Engine::QVS_SetGainPointY2(point_value);
		}

		point_value = -1.f;
		sscanf(p3_x->value(), "%lf", &point_value);
		if (point_value < 0) {
			p3_x->value("0");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = 0.0f;
			QVS_Engine::QVS_SetGainPointX3(0.0f);
		}
		else if (point_value > 255) {
			p3_x->value("255");
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = 255.0f;
			QVS_Engine::QVS_SetGainPointX3(255.0f);
		}
		else {
			//manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = point_value;
			QVS_Engine::QVS_SetGainPointX3(point_value);
		}
		point_value = -1.f;
		sscanf(p3_y->value(), "%lf", &point_value);
		if (point_value < 0) {
			p3_y->value("0");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = 0.0f;
			QVS_Engine::QVS_SetGainPointY3(0.0f);
		}
		else if (point_value > 255) {
			p3_y->value("255");
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = 255.0f;
			QVS_Engine::QVS_SetGainPointY3(255.0f);
		}
		else {
			//manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][2] = point_value;
			QVS_Engine::QVS_SetGainPointY3(point_value);
		}

		/*
		std::vector<std::vector<std::vector<cv::Point2d>>> channels;
		std::vector<std::vector<cv::Point2d>> rgb[MAX_CAMERA];
		std::vector<cv::Point2d> points[3];

		for (int i = 0; i < manager->mNumCamera; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					cv::Point2d gain_curve_point(manager->mGainPointX[i][j][k], manager->mGainPointY[i][j][k]);
					points[j].push_back(gain_curve_point);
					printf("(%.2lf,%.2lf)", points[j].at(k).x, points[j].at(k).y);
				}
				rgb[j].push_back(points[j]);
				printf("\t");
			}
			channels.push_back(rgb[i]);
			printf("\n");
		}
		*/

		/*
		Fl_Group* gain_curve_points = (Fl_Group*)findChildByLabel(btn->parent(), "gain_curve_points");
		Fl_Group* point_round_button = (Fl_Group*)findChildByLabel(gain_curve_points, "point_round_button");
		Fl_Round_Button* btn_input_p1 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P1");
		Fl_Round_Button* btn_input_p2 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P2");
		Fl_Round_Button* btn_input_p3 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P3");
		btn_input_p1->value(0);
		btn_input_p2->value(0);
		btn_input_p3->value(0);
		Fl_Group* p1 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point1");
		Fl_Float_Input* p1_x = (Fl_Float_Input*)findChildByLabel(p1, "point_x");
		Fl_Float_Input* p1_y = (Fl_Float_Input*)findChildByLabel(p1, "point_y");
		p1_x->value("");
		p1_y->value("");
		Fl_Group* p2 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point2");
		Fl_Float_Input* p2_x = (Fl_Float_Input*)findChildByLabel(p2, "point_x");
		Fl_Float_Input* p2_y = (Fl_Float_Input*)findChildByLabel(p2, "point_y");
		p2_x->value("");
		p2_y->value("");
		Fl_Group* p3 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point3");
		Fl_Float_Input* p3_x = (Fl_Float_Input*)findChildByLabel(p3, "point_x");
		Fl_Float_Input* p3_y = (Fl_Float_Input*)findChildByLabel(p3, "point_y");
		p3_x->value("");
		p3_y->value("");
		gain_curve_points->deactivate();

		Fl_Group* channel_round_button = (Fl_Group*)findChildByLabel(btn->parent(), "channel_round_button");
		Fl_Round_Button* btn_input1 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "1");
		Fl_Round_Button* btn_input2 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "2");
		Fl_Round_Button* btn_input3 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "3");
		Fl_Round_Button* btn_input4 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "4");
		Fl_Round_Button* btn_input5 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "5");
		Fl_Round_Button* btn_input6 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "6");
		btn_input1->value(0);
		btn_input2->value(0);
		btn_input3->value(0);
		btn_input4->value(0);
		btn_input5->value(0);
		btn_input6->value(0);

		Fl_Group* rgb_round_button = (Fl_Group*)findChildByLabel(btn->parent(), "rgb_round_button");
		Fl_Round_Button* btn_input_r = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "R");
		Fl_Round_Button* btn_input_g = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "G");
		Fl_Round_Button* btn_input_b = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "B");
		btn_input_r->value(0);
		btn_input_g->value(0);
		btn_input_b->value(0);
		rgb_round_button->deactivate();

		manager->mGainChannelIndex = 0;
		manager->mGainRGBIndex = 0;
		for (int i = 0; i < MAX_CAMERA; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					manager->mGainPointX[i][j][k] = -1.f;
					manager->mGainPointY[i][j][k] = -1.f;
				}
			}
		}
		*/

		//manager->doManualGainMode();
		QVS_Engine::QVS_DoManualGainMode();
	}

	// round button
	if (btn->label() == "1" || btn->label() == "2" || btn->label() == "3" || btn->label() == "4" || btn->label() == "5" || btn->label() == "6") {
		int index;
		sscanf(btn->label(), "%d", &index);
		//manager->mGainChannelIndex = index - 1;
		QVS_Engine::QVS_SetGainChannelIndex(index - 1);

		Fl_Group* gain_curve_info = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "gain_curve_info");
		Fl_Box* gain_channel_text_box = (Fl_Box*)gain_curve_info->child(0);
		char* channel_text = (char*)malloc(256);
		sprintf(channel_text, "Input : %s   ", btn->label());
		gain_channel_text_box->label(channel_text);

		Fl_Box* gain_rgb_text_box = (Fl_Box*)gain_curve_info->child(1);
		char* rgb_text = (char*)malloc(256);
		sprintf(rgb_text, "Color :     ");
		gain_rgb_text_box->label(rgb_text);

		Fl_Box* gain_point_text_box = (Fl_Box*)gain_curve_info->child(2);
		char* point_text = (char*)malloc(256);
		sprintf(point_text, "Point :     ");
		gain_point_text_box->label(point_text);

		Fl_Group* rgb_round_button = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "rgb_round_button");
		rgb_round_button->activate();

		Fl_Round_Button* btn_input_r = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "R");
		Fl_Round_Button* btn_input_g = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "G");
		Fl_Round_Button* btn_input_b = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "B");
		btn_input_r->value(0);
		btn_input_g->value(0);
		btn_input_b->value(0);

		Fl_Group* gain_curve_points = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "gain_curve_points");
		Fl_Group* point_round_button = (Fl_Group*)findChildByLabel(gain_curve_points, "point_round_button");
		Fl_Round_Button* btn_input_p1 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P1");
		Fl_Round_Button* btn_input_p2 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P2");
		Fl_Round_Button* btn_input_p3 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P3");
		btn_input_p1->value(0);
		btn_input_p2->value(0);
		btn_input_p3->value(0);
		Fl_Group* p1 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point1");
		Fl_Float_Input* p1_x = (Fl_Float_Input*)findChildByLabel(p1, "point_x");
		Fl_Float_Input* p1_y = (Fl_Float_Input*)findChildByLabel(p1, "point_y");
		p1_x->value("");
		p1_y->value("");
		Fl_Group* p2 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point2");
		Fl_Float_Input* p2_x = (Fl_Float_Input*)findChildByLabel(p2, "point_x");
		Fl_Float_Input* p2_y = (Fl_Float_Input*)findChildByLabel(p2, "point_y");
		p2_x->value("");
		p2_y->value("");
		Fl_Group* p3 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point3");
		Fl_Float_Input* p3_x = (Fl_Float_Input*)findChildByLabel(p3, "point_x");
		Fl_Float_Input* p3_y = (Fl_Float_Input*)findChildByLabel(p3, "point_y");
		p3_x->value("");
		p3_y->value("");
		gain_curve_points->deactivate();

		Fl_Button* btn_apply = (Fl_Button*)findChildByLabel(btn->parent()->parent(), "Apply");
		btn_apply->deactivate();

		//manager->doManualGainMode();
		QVS_Engine::QVS_DoManualGainMode();
	}
	else if (btn->label() == "R" || btn->label() == "G" || btn->label() == "B") {
		char temp_w[256];
		int index;
		if (btn->label() == "R")
			index = 0;
		else if (btn->label() == "G")
			index = 1;
		else if (btn->label() == "B")
			index = 2;

		//manager->mGainRGBIndex = index;
		QVS_Engine::QVS_SetGainRGBIndex(index);

		Fl_Group* gain_curve_info = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "gain_curve_info");
		Fl_Box* gain_rgb_text_box = (Fl_Box*)gain_curve_info->child(1);
		char* rgb_text = (char*)malloc(256);
		sprintf(rgb_text, "Color : %s   ", btn->label());
		gain_rgb_text_box->label(rgb_text);

		Fl_Box* gain_point_text_box = (Fl_Box*)gain_curve_info->child(2);
		char* point_text = (char*)malloc(256);
		sprintf(point_text, "Point :     ");
		gain_point_text_box->label(point_text);

		Fl_Group* gain_curve_points = (Fl_Group*)findChildByLabel(btn->parent()->parent(), "gain_curve_points");
		gain_curve_points->activate();

		Fl_Group* point_round_button = (Fl_Group*)findChildByLabel(gain_curve_points, "point_round_button");
		Fl_Round_Button* btn_input_p1 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P1");
		Fl_Round_Button* btn_input_p2 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P2");
		Fl_Round_Button* btn_input_p3 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P3");
		btn_input_p1->value(0);
		btn_input_p2->value(0);
		btn_input_p3->value(0);

		Fl_Group* p1 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point1");
		p1->activate();
		Fl_Float_Input* p1_x = (Fl_Float_Input*)findChildByLabel(p1, "point_x");
		Fl_Float_Input* p1_y = (Fl_Float_Input*)findChildByLabel(p1, "point_y");
		//if (manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][0] >= 0 && manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][0] <= 255) {
		if (QVS_Engine::QVS_GetGainPointX1() >= 0 && QVS_Engine::QVS_GetGainPointX1() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointX1());
			p1_x->value(temp_w);
		}
		else {
			p1_x->value("");
		}
		//if (manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][0] >= 0 && manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][0] <= 255) {
		if (QVS_Engine::QVS_GetGainPointY1() >= 0 && QVS_Engine::QVS_GetGainPointY1() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointY1());
			p1_y->value(temp_w);
		}
		else {
			p1_y->value("");
		}

		Fl_Group* p2 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point2");
		p2->activate();
		Fl_Float_Input* p2_x = (Fl_Float_Input*)findChildByLabel(p2, "point_x");
		Fl_Float_Input* p2_y = (Fl_Float_Input*)findChildByLabel(p2, "point_y");
		//if (manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][1] >= 0 && manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][1] <= 255) {
		if (QVS_Engine::QVS_GetGainPointX2() >= 0 && QVS_Engine::QVS_GetGainPointX2() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointX2());
			p2_x->value(temp_w);
		}
		else {
			p2_x->value("");
		}
		//if (manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][1] >= 0 && manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][1] <= 255) {
		if (QVS_Engine::QVS_GetGainPointY2() >= 0 && QVS_Engine::QVS_GetGainPointY2() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointY2());
			p2_y->value(temp_w);
		}
		else {
			p2_y->value("");
		}

		Fl_Group* p3 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point3");
		p3->activate();
		Fl_Float_Input* p3_x = (Fl_Float_Input*)findChildByLabel(p3, "point_x");
		Fl_Float_Input* p3_y = (Fl_Float_Input*)findChildByLabel(p3, "point_y");
		//if (manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][2] >= 0 && manager->mGainPointX[manager->mGainChannelIndex][manager->mGainRGBIndex][2] <= 255) {
		if (QVS_Engine::QVS_GetGainPointX3() >= 0 && QVS_Engine::QVS_GetGainPointX3() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointX3());
			p3_x->value(temp_w);
		}
		else {
			p3_x->value("");
		}
		//if (manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][2] >= 0 && manager->mGainPointY[manager->mGainChannelIndex][manager->mGainRGBIndex][2] <= 255) {
		if (QVS_Engine::QVS_GetGainPointY3() >= 0 && QVS_Engine::QVS_GetGainPointY3() <= 255) {
			sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetGainPointY3());
			p3_y->value(temp_w);
		}
		else {
			p3_y->value("");
		}

		Fl_Button* btn_apply = (Fl_Button*)findChildByLabel(btn->parent()->parent(), "Apply");
		btn_apply->activate();
	}
	else if (btn->label() == "P1" || btn->label() == "P2" || btn->label() == "P3") {
		int index;
		sscanf(btn->label(), "P%d", &index);
		//manager->mGainPointIndex = index - 1;
		QVS_Engine::QVS_SetGainPointIndex(index - 1);

		Fl_Group* gain_curve_info = (Fl_Group*)findChildByLabel(btn->parent()->parent()->parent(), "gain_curve_info");
		Fl_Box* gain_point_text_box = (Fl_Box*)gain_curve_info->child(2);
		char* point_text = (char*)malloc(256);
		sprintf(point_text, "Point : %d   ", index);
		gain_point_text_box->label(point_text);
	}
}

void mainmenu_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* maincontent = (Fl_Group*)findChildByLabel(content, "maincontent");
	Fl_Group* subcontent = (Fl_Group*)findChildByLabel(content, "subcontent");
	Fl_Group* submenu = (Fl_Group*)findChildByLabel(content, "submenu");
	GLWin* gl_main_win = win->gl_main_win;
	EngineManager* manager = (EngineManager*)gl_main_win->engineManager;

	// Change Sub Menu Text
	if (btn->label() == "Sources") {
		changePreviewMode(TAB_MODE_SOURCES, win);
	}
	else if (btn->label() == "Panorama") {
		changePreviewMode(TAB_MODE_OUTPUT, win);
	}
	else if (btn->label() == "Interactive") {
		changePreviewMode(TAB_MODE_INTERACTIVE, win);
	}
	else if (btn->label() == "Configuration") {
		changePreviewMode(TAB_MODE_CONFIGURATION, win);
	}

	// Change Configuration Text
	if (btn->label() == "Configuration") {
		Fl_Button* btn1 = (Fl_Button*)(maincontent->child(2));
		btn1->label("Outputs");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(maincontent->child(3));
		btn2->label("Panorama");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(maincontent->child(4));
		btn3->label("Calibration");
		btn3->show();
	}
	else {
		Fl_Button* btn1 = (Fl_Button*)(maincontent->child(2));
		btn1->label("");
		btn1->hide();

		Fl_Button* btn2 = (Fl_Button*)(maincontent->child(3));
		btn2->label("");
		btn2->hide();

		Fl_Button* btn3 = (Fl_Button*)(maincontent->child(4));
		btn3->label("");
		btn3->hide();
	}

	// Change GL
	if (gl_main_win) {
		if (btn->label() != "Configuration") {
			maincontent->hide();
			//gl_main_win->show();

			int x = maincontent->x() + 10;
			int y = maincontent->y() + 10;
			int w = maincontent->w() - 20;
			int h = maincontent->h() - 20;
			gl_main_win->resize(x, y, w, h);
			//gl_main_win->redraw();

			findChildByLabel(subcontent, "OUTPUT OFF", "OUTPUT ON")->activate();

			Fl_Button* btn3 = (Fl_Button*)findChildByLabel(subcontent, "Normal Blending", "Multiband Blending");
			btn3->activate();

			Fl_Button* btn4 = (Fl_Button*)findChildByLabel(subcontent, "Seam Image", "Seam Dist");
			btn4->activate();

			Fl_Button* btn5 = (Fl_Button*)findChildByLabel(subcontent, "Nadir Off", "Nadir On");
			btn5->activate();

			Fl_Button* btn6 = (Fl_Button*)findChildByLabel(subcontent, "Nadir BOTTOM", "Nadir TOP");
			btn6->activate();

			Fl_Counter* btn7 = (Fl_Counter*)findChildByLabel(subcontent, "counter_size");
			btn7->activate();

			Fl_Button* btn8 = (Fl_Button*)findChildByLabel(subcontent, "Browse...");
			btn8->activate();

			Fl_Button* btn9 = (Fl_Button*)findChildByLabel(subcontent, "Color Setting");
			btn9->activate();

			Fl_Choice* choice1 = (Fl_Choice*)findChildByLabel(subcontent, "choice_gain");
			choice1->activate();

			Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
			video->activate();

			Fl_Button* btn12 = (Fl_Button*)findChildByLabel(video, "@<");
			btn12->activate();

			Fl_Button* btn13 = (Fl_Button*)findChildByLabel(video, "@>");
			btn13->activate();

			Fl_Output* output2 = (Fl_Output*)findChildByLabel(video, "sec");
			output2->activate();

			Fl_Button* btn14 = (Fl_Button*)findChildByLabel(subcontent, "Auto Gain On", "Auto Gain Off");
			btn14->activate();

			Fl_Button* btn15 = (Fl_Button*)findChildByLabel(subcontent, "Curve Setting");
			if (choice1->value() == 3)
				btn15->activate();
		}
		else {
			gl_main_win->resize(0, 0, 0, 0);
			//gl_main_win->fullscreen_off();
			maincontent->show();

			findChildByLabel(subcontent, "OUTPUT OFF", "OUTPUT ON")->deactivate();

			Fl_Button* btn3 = (Fl_Button*)findChildByLabel(subcontent, "Normal Blending", "Multiband Blending");
			btn3->deactivate();

			Fl_Button* btn4 = (Fl_Button*)findChildByLabel(subcontent, "Seam Image", "Seam Dist");
			btn4->deactivate();

			Fl_Button* btn5 = (Fl_Button*)findChildByLabel(subcontent, "Nadir Off", "Nadir On");
			btn5->deactivate();

			Fl_Button* btn6 = (Fl_Button*)findChildByLabel(subcontent, "Nadir BOTTOM", "Nadir TOP");
			btn6->deactivate();

			Fl_Counter* btn7 = (Fl_Counter*)findChildByLabel(subcontent, "counter_size");
			btn7->deactivate();

			Fl_Button* btn8 = (Fl_Button*)findChildByLabel(subcontent, "Browse...");
			btn8->deactivate();

			Fl_Button* btn9 = (Fl_Button*)findChildByLabel(subcontent, "Color Setting");
			btn9->deactivate();

			Fl_Choice* choice1 = (Fl_Choice*)findChildByLabel(subcontent, "choice_gain");
			choice1->deactivate();

			Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
			video->deactivate();

			Fl_Button* btn12 = (Fl_Button*)findChildByLabel(video, "@<");
			btn12->deactivate();

			Fl_Button* btn13 = (Fl_Button*)findChildByLabel(video, "@>");
			btn13->deactivate();

			Fl_Output* output2 = (Fl_Output*)findChildByLabel(video, "sec");
			output2->deactivate();

			Fl_Button* btn14 = (Fl_Button*)findChildByLabel(subcontent, "Auto Gain On", "Auto Gain Off");
			btn14->deactivate();

			Fl_Button* btn15 = (Fl_Button*)findChildByLabel(subcontent, "Curve Setting");
			btn15->deactivate();
		}
	}
}

void submenu_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* submenu = (Fl_Group*)findChildByLabel(content, "submenu");

	// main menu : Sources
	if (btn->label() == "Edit Input") {
		char temp_w[256];

		Fl_Group* capture_config = (Fl_Group*)findChildByLabel(win->edit_input_win, "capture_config");

		for (int i = 0; i < CAMERA_MAX; i++) {
			Fl_Choice* camera_index_choice = (Fl_Choice*)findChildByLabel(capture_config, camera_index_choice_label[i]);
			camera_index_choice->clear();
			camera_index_choice->add("No Device");
			for (int j = 0; j < QVS_Engine::QVS_getCaptureDeviceSize(); j++) {
				camera_index_choice->add(QVS_Engine::QVS_getCaptureDeviceName(j));
			}
			camera_index_choice->add("...Image");
			//camera_index_choice->add("...Movie");
			if (i < QVS_Engine::QVS_GetNumCameara())
				camera_index_choice->show();
			else
				camera_index_choice->hide();

			camera_index_choice->value(QVS_Engine::QVS_getCaptureDeviceIndex(i) + 1);
		}

		Fl_Group* input_parameter_config = (Fl_Group*)findChildByLabel(win->edit_input_win, "input_parameter_config");

		Fl_Input* fps_input = (Fl_Input*)findChildByLabel(input_parameter_config, "fps_input");
		sprintf(temp_w, "%.2f", QVS_Engine::QVS_GetCapureFPS());
		fps_input->value(temp_w);

		Fl_Input* width_input = (Fl_Input*)findChildByLabel(input_parameter_config, "width_input");
		sprintf(temp_w, "%d", QVS_Engine::QVS_GetCapureWidth());
		width_input->value(temp_w);

		Fl_Input* height_input = (Fl_Input*)findChildByLabel(input_parameter_config, "height_input");
		sprintf(temp_w, "%d", QVS_Engine::QVS_GetCapureHeight());
		height_input->value(temp_w);

		Fl_Choice* choice_lens = (Fl_Choice*)findChildByLabel(input_parameter_config, "choice_lens");
		{
			sprintf(temp_w, "%d", QVS_Engine::QVS_getLensType());
			choice_lens->value(QVS_Engine::QVS_getLensType());
		}

		Fl_Input* fov_input = (Fl_Input*)findChildByLabel(input_parameter_config, "fov_input");
		sprintf(temp_w, "%f", QVS_Engine::QVS_getFOV());
		fov_input->value(temp_w);

		win->edit_input_win->show();
	}
	else if (btn->label() == "Snapshot") {
		//manager->SnapShot(); //delay settime
		//QVS_Engine::QVS_setCallback_snapshot(onCallbackDone);
		QVS_Engine::QVS_SnapShot();

	}
	// main menu : Panorama & Interactive
	else if (btn->label() == "Pano Save") {
		//QVS_Engine::QVS_savePanorama();
	}
	// calibration menu
	else if (btn->label() == "Calibration") {
		if (win->edit_mode == 1) {
			win->edit_mode = 0;
			btn->label("Edit Off");
			QVS_Engine::QVS_saveOutput();

			Fl_Button* btn5 = (Fl_Button*)(submenu->child(5));
			btn5->label("Straighten");
			btn5->hide();
		}
		win->gl_main_win->global_tab_mode = TAB_MODE_OUTPUT;

		win->calibration_menu = 1;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Align\nCalibration");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("Update\nCalibration");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("Advance");
		btn3->show();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("@<");
		btn4->show();
	}
	else if (btn->label() == "Align\nCalibration") {
		QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_MakeTemplate);
	}
	else if (btn->label() == "Detail\nCalibration") {
		QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_Detail_All);
	}
	else if (btn->label() == "Fast\nCalibration") {
		//QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_All);
	}
	else if (btn->label() == "Advanced\nCalibration") {
		QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_Advanced);
	}
	else if (btn->label() == "Update\nCalibration") {
		win->calibration_menu = 2;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Fast\nCalibration");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("Detail\nCalibration");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("@<");
		btn3->show();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("");
		btn4->hide();
	}
	else if (btn->label() == "Advance") {
		win->calibration_menu = 2;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Select Mode");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("Mask Mode");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("Advanced\nCalibration");
		btn3->show();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("@<");
		btn4->show();
	}
	else if (btn->label() == "Select Mode") {
		win->select_mode = 1;
		win->calibration_menu = 3;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Select All");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("@<");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("");
		btn3->hide();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("");
		btn4->hide();

		QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_On);
	}
	else if (btn->label() == "Mask Mode") {
		win->mask_mode = 1;
		win->calibration_menu = 3;

		Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
		btn1->label("Clear");
		btn1->show();

		Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
		btn2->label("@<");
		btn2->show();

		Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
		btn3->label("");
		btn3->hide();

		Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
		btn4->label("");
		btn4->hide();

		QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_On);
	}
	else if (btn->label() == "Select All") {
		QVS_Engine::QVS_selectAllROI();
	}
	else if (btn->label() == "Clear") {
		QVS_Engine::QVS_clearROI();
	}
	else if (btn->label() == "@<") {
		if (win->calibration_menu > 0)
			win->calibration_menu--;

		if (win->calibration_menu == 0) {
			changePreviewMode(TAB_MODE_OUTPUT, win);
		}
		else if (win->calibration_menu == 1) {
			Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
			btn1->label("Align\nCalibration");
			btn1->show();

			Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
			btn2->label("Update\nCalibration");
			btn2->show();

			Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
			btn3->label("Advance");
			btn3->show();

			Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
			btn4->label("@<");
			btn4->show();
		}
		else if (win->calibration_menu == 2) {
			Fl_Button* btn1 = (Fl_Button*)(submenu->child(2));
			btn1->label("Select Mode");
			btn1->show();

			Fl_Button* btn2 = (Fl_Button*)(submenu->child(3));
			btn2->label("Mask Mode");
			btn2->show();

			Fl_Button* btn3 = (Fl_Button*)(submenu->child(4));
			btn3->label("Advanced\nCalibration");
			btn3->show();

			Fl_Button* btn4 = (Fl_Button*)(submenu->child(5));
			btn4->label("@<");
			btn4->show();

			win->select_mode = 0;
			win->mask_mode = 0;
			QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
			QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);
		}
	}
}

void configuration_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* subcontent = (Fl_Group*)findChildByLabel(content, "subcontent");
	EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;

	// start menu
	if (btn->label() == "New\n\nConfiguration") {
		win->new_configuration_win->show();

		win->init_mode_label = btn->label();
	}
	else if (btn->label() == "Load\n\nConfiguration") {
		//win->new_configuration_win->show();

		//win->init_mode_label = btn->label();

		char path[256];
		Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
		chooser->title("Open 360 Camera configuration");
		chooser->filter("SKT 360 Video File\t*.q360");
		chooser->directory(".\\");
		if (chooser->show() == 0) {
			sprintf(path, "%s", chooser->filename());

			int number_of_camera;
			int width;
			int height;
			int output_width = 0;
			int output_height = 0;

			wchar_t strUnicode[256] = { 0, };
			char strMultiByte[256] = { 0, };
			MultiByteToWideChar(CP_UTF8, 0, path, strlen(path), strUnicode, 256);
			WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

			FILE* fp = fopen(strMultiByte, "rt");

			if (fp == NULL)
			{
				printf("Load ERROR: PTS file open fail!\n");
				return;
			}

			char temp_w[MAX_PATH];

			bool isMore;

			// status
			isMore = false;

			while (fscanf(fp, "%s", temp_w) != EOF)
			{
				if (strcmp(temp_w, "#-new") == 0)
				{
					isMore = true;
					break;
				}
			}

			while (isMore)
			{
				if (fscanf(fp, "%s", temp_w) == EOF)
				{
					printf("Load ERROR: Not found configuration value!\n");
					fclose(fp);
					return;
				}

				switch (temp_w[0])
				{
				case 'n':
					sscanf(temp_w, "n %d", &(number_of_camera));
					break;
				case 'w':
					sscanf(temp_w, "w %d", &(width));
					break;
				case 'h':
					sscanf(temp_w, "h %d", &(height));
					//isMore = false;
					break;

				case 'x':
					//output width
					sscanf(temp_w, "x %d", &(output_width));
					break;
				case 'y':
					//output height
					sscanf(temp_w, "y %d", &(output_height));
					isMore = false;
					break;
				default:
					break;
				}
				if (strcmp(temp_w, "#-input") == 0)
				{
					break;
				}
					
			}
			if (fp) fclose(fp);

			printf("JW configuration_btn_callback LoadConfig: (output_width, output_height) = (%d, %d)\n", output_width, output_height);
			if (output_width == 0 || output_height == 0)
			{
				output_width = 3840;
				output_height = 1920;
				printf("JW configuration_btn_callback LoadConfig exception!!: (output_width, output_height) = (%d, %d)\n", output_width, output_height);
			}
			//win->gl_main_win->engineManager = new EngineManager();
			//win->gl_main_win->engineManager->Create(capture_type, number_of_camera, channelIndex, width, height);
			QVS_Engine::QVS_Create(number_of_camera, width, height, output_width, output_height, QVS_Engine::StitchingMode_Stereo);
			win->gl_main_win->engineReady = TRUE;
			setCallbacks();

			//EngineManager* manager = (EngineManager*)win->gl_main_win->engineManager;
			//if (manager->LoadParam(path) == 0) {
			if (QVS_Engine::QVS_LoadParam(path) == 0) {
				fl_message("Load Configuration Fail!");
				return;
			}

			char *temp = strtok((char*)chooser->filename(), "\\");
			char *subject_name;

			while (temp != NULL) {
				subject_name = temp;
				temp = strtok(NULL, "\\");
			}

			subject_name = strtok(subject_name, ".");

			Fl_Group* subjectbar = (Fl_Group*)findChildByLabel(win, "subjectbar");
			subjectbar->child(2)->label(subject_name);

			Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
			Fl_Widget* btn_Sources = findChildByLabel(mainmenu, "Sources");
			Fl_Widget* btn_Panorama = findChildByLabel(mainmenu, "Panorama");
			Fl_Widget* btn_Interactive = findChildByLabel(mainmenu, "Interactive");
			Fl_Widget* btn_Configuration = findChildByLabel(mainmenu, "Configuration");

			btn_Sources->show();
			btn_Panorama->show();
			btn_Interactive->show();
			btn_Configuration->show();

			Fl_Button* btn_exposure_mode = (Fl_Button*)findChildByLabel(subcontent, "Normal Blending", "Multiband Blending");
			//if (manager->mBlendingMode == GRADATION_BLEND)
			if (QVS_Engine::QVS_GetExposureMode() == NORMAL_BLEND)
				btn_exposure_mode->label("Normal Blending");
			//else if (manager->mBlendingMode == LAPLACIAN_BLEND)
			else if (QVS_Engine::QVS_GetExposureMode() == MULTIBAND_BLEND)
				btn_exposure_mode->label("Multiband Blending");

			Fl_Button* btn_seam_mode = (Fl_Button*)findChildByLabel(subcontent, "Seam Image", "Seam Dist");
			//if (manager->mSeamMode == FIND_SEAM_FROM_IMAGE)
			if (QVS_Engine::QVS_GetSeamMode() == FIND_SEAM_FROM_IMAGE)
				btn_seam_mode->label("Seam Image");
			//else if (manager->mSeamMode == FIND_SEAM_FROM_DIST)
			else if (QVS_Engine::QVS_GetSeamMode() == FIND_SEAM_FROM_DIST)
				btn_seam_mode->label("Seam Dist");

			Fl_Choice* choice_gain = (Fl_Choice*)findChildByLabel(subcontent, "choice_gain");
			//choice_gain->value(manager->mGainMode);
			choice_gain->value(QVS_Engine::QVS_GetGainMode());

			Fl_Button* btn_nadir_mode = (Fl_Button*)findChildByLabel(subcontent, "Nadir Off", "Nadir On");
			//if (manager->mNadirMode == NADIR_LOG_OFF)
			if (QVS_Engine::QVS_GetNadirMode() == NADIR_LOG_OFF)
				btn_nadir_mode->label("Nadir Off");
			//else if (manager->mNadirMode == NADIR_LOG_ON)
			else if (QVS_Engine::QVS_GetNadirMode() == NADIR_LOG_ON)
				btn_nadir_mode->label("Nadir On");

			Fl_Button* btn_nadir_position = (Fl_Button*)findChildByLabel(subcontent, "Nadir BOTTOM", "Nadir TOP");
			//if (manager->mNadirPosition == NADIR_BOTTOM)
			if (QVS_Engine::QVS_GetNadirPosition() == NADIR_BOTTOM)
				btn_nadir_position->label("Nadir BOTTOM");
			//else if (manager->mNadirPosition == NADIR_TOP)
			else if (QVS_Engine::QVS_GetNadirPosition() == NADIR_TOP)
				btn_nadir_position->label("Nadir TOP");

			Fl_Counter* counter_size = (Fl_Counter*)findChildByLabel(subcontent, "counter_size");
			//counter_size->value(manager->mNadirSize);
			counter_size->value(QVS_Engine::QVS_GetNadirSize());

			mainmenu_btn_callback(btn_Sources, win);
		}
	}
	// configuration menu
	else if (btn->label() == "Outputs") {
		//I'll be back... 20161104
		{
			char temp_w[256];

			Fl_Group* bmoutput_settings = (Fl_Group*)findChildByLabel(win->outputs_win, "bmoutput_settings");

			Fl_Choice* device_choice = (Fl_Choice*)findChildByLabel(bmoutput_settings, "device_choice");
			device_choice->clear();
			device_choice->add("No device");
			int i = 0;
			while (true) {
				i++;
				char displayName[256];
				char modelName[256];
				if (QVS_Engine::QVS_GetDeviceList(i, displayName, modelName) == false) {
					break;
				}

				device_choice->add(displayName);
			}
			device_choice->value(QVS_Engine::QVS_GetBmOutputDevice());

			Fl_Choice* display_choice = (Fl_Choice*)findChildByLabel(bmoutput_settings, "display_choice");
			display_choice->clear();
			display_choice->add("Default Display (3840x2160 30fps)");
			display_choice->value(QVS_Engine::QVS_GetBmOutputDisplay());
		}

		win->outputs_win->show();
	}
	else if (btn->label() == "Panorama") {
		Fl_Group* panorama_settings = (Fl_Group*)findChildByLabel(win->panorama_win, "panorama_settings");

		Fl_Choice* size_choice = (Fl_Choice*)findChildByLabel(panorama_settings, "size_choice");
		//size_choice->value(manager->mPanoramaSize);
		size_choice->value(QVS_Engine::QVS_GetPanoramaSize());

		win->panorama_win->show();
	}
	else if (btn->label() == "Calibration") {
		Fl_Group* setting_calibration = (Fl_Group*)findChildByLabel(win->calibration_win, "setting_calibration");

		Fl_Input* snapshot_input = (Fl_Input*)findChildByLabel(setting_calibration, "snapshot_input");
		//snapshot_input->value(manager->snapshot_path);
		snapshot_input->value(QVS_Engine::QVS_GetSnapshot_path());

		win->calibration_win->show();
	}
}

void subcontent_btn_callback(Fl_Widget* w, void* data) {
	Fl_Button* btn = (Fl_Button*)w;
	Quram_Fl_Window* win = (Quram_Fl_Window*)data;
	Fl_Group* content = (Fl_Group*)findChildByLabel(win, "content");
	Fl_Group* subcontent = (Fl_Group*)findChildByLabel(content, "subcontent");
	GLWin* gl_main_win = win->gl_main_win;
	EngineManager* manager = (EngineManager*)gl_main_win->engineManager;

	if (btn->label() == "OUTPUT OFF" || btn->label() == "OUTPUT ON") {
		if (win->output_mode == 0)
		{
			//QVS_Engine::QVS_setCallback_startOutput(onCallbackDone);
			//if (manager->startOutput() == 1)
			if (QVS_Engine::QVS_StartOutput() == 1)
			{
				win->output_mode = 1;
				btn->label("OUTPUT ON");

				Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
				Fl_Widget* btn_Configuration = findChildByLabel(mainmenu, "Configuration");
				btn_Configuration->deactivate();
			}
		}
		else if (win->output_mode == 1)
		{
			//if (manager->finishOutput() == 1)
			if (QVS_Engine::QVS_FinishOutput() == 1)
			{
				win->output_mode = 0;
				btn->label("OUTPUT OFF");

				if (win->output_mode != 1) {
					Fl_Group* mainmenu = (Fl_Group*)findChildByLabel(win, "mainmenu");
					Fl_Widget* btn_Configuration = findChildByLabel(mainmenu, "Configuration");
					if (win->edit_mode == 0)
						btn_Configuration->activate();
				}
			}
		}
	}
	else if (btn->label() == "Multiband Blending") {
		//manager->setBlendingMode(GRADATION_BLEND);
		QVS_Engine::QVS_SetExposureCompensationMode(NORMAL_BLEND);
		btn->label("Normal Blending");
	}
	else if (btn->label() == "Normal Blending") {
		//manager->setBlendingMode(LAPLACIAN_BLEND);
		QVS_Engine::QVS_SetExposureCompensationMode(MULTIBAND_BLEND);
		btn->label("Multiband Blending");
	}
	else if (btn->label() == "Seam Dist") {
		//QVS_Engine::QVS_setCallback_seamMode(onCallbackDone);
		//manager->setSeamMode(FIND_SEAM_FROM_IMAGE);
		QVS_Engine::QVS_SetSeamMode(FIND_SEAM_FROM_IMAGE);
		btn->label("Seam Image");
	}
	else if (btn->label() == "Seam Image") {
		//QVS_Engine::QVS_setCallback_seamMode(onCallbackDone);
		//manager->setSeamMode(FIND_SEAM_FROM_DIST);
		QVS_Engine::QVS_SetSeamMode(FIND_SEAM_FROM_DIST);
		btn->label("Seam Dist");
	}
	else if (btn->label() == "choice_gain") {
		Fl_Choice* choice = (Fl_Choice*)btn;
		Fl_Button* btn_curve_setting = (Fl_Button*)findChildByLabel(btn->parent(), "Curve Setting");

		//QVS_Engine::QVS_setCallback_gainMode(onCallbackDone);
		//manager->setGainMode(choice->value());
		QVS_Engine::QVS_SetGainMode(choice->value());
		//printf("choice->value()=%d\n", choice->value());

		if (choice->value() == 3) {
			btn_curve_setting->activate();
		}
		else {
			btn_curve_setting->deactivate();
			win->gain_curve_manual_win->hide();
		}
	}
	else if (btn->label() == "Color Setting") {
		Fl_Group* color_setting = (Fl_Group*)findChildByLabel(win->color_setting_win, "color_setting");
		Fl_Group* round_button = (Fl_Group*)findChildByLabel(color_setting, "round_button");
		
		//if (manager->mRGBIndex == 0) {
		if (QVS_Engine::QVS_GetRGBIndex() == 0) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "1");
			btn->setonly();
		}
		//else if (manager->mRGBIndex == 1) {
		else if (QVS_Engine::QVS_GetRGBIndex() == 1) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "2");
			btn->setonly();
		}
		//else if (manager->mRGBIndex == 2) {
		else if (QVS_Engine::QVS_GetRGBIndex() == 2) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "3");
			btn->setonly();
		}
		//else if (manager->mRGBIndex == 3) {
		else if (QVS_Engine::QVS_GetRGBIndex() == 3) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "4");
			btn->setonly();
		}
		//else if (manager->mRGBIndex == 4) {
		else if (QVS_Engine::QVS_GetRGBIndex() == 4) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "5");
			btn->setonly();
		}
		//else if (manager->mRGBIndex == 5) {
		else if (QVS_Engine::QVS_GetRGBIndex() == 5) {
			Fl_Round_Button* btn = (Fl_Round_Button*)findChildByLabel(round_button, "6");
			btn->setonly();
		}

		Fl_Value_Slider* slider_red = (Fl_Value_Slider*)findChildByLabel(color_setting, "RED");
		Fl_Value_Slider* slider_green = (Fl_Value_Slider*)findChildByLabel(color_setting, "GREEN");
		Fl_Value_Slider* slider_blue = (Fl_Value_Slider*)findChildByLabel(color_setting, "BLUE");

		//slider_red->value(manager->mRed[manager->mRGBIndex]);
		//slider_green->value(manager->mGreen[manager->mRGBIndex]);
		//slider_blue->value(manager->mBlue[manager->mRGBIndex]);
		slider_red->value(QVS_Engine::QVS_GetRed());
		slider_green->value(QVS_Engine::QVS_GetGreen());
		slider_blue->value(QVS_Engine::QVS_GetBlue());

		win->color_setting_win->show();
	}
	else if (btn->label() == "Curve Setting") {
		if (!win->gain_curve_manual_win->shown()) {
			Fl_Group* gain_curve_manual = (Fl_Group*)findChildByLabel(win->gain_curve_manual_win, "gain_curve_manual");

			Fl_Group* gain_curve_info = (Fl_Group*)findChildByLabel(gain_curve_manual, "gain_curve_info");
			Fl_Box* gain_channel_text_box = (Fl_Box*)gain_curve_info->child(0);
			char* channel_text = (char*)malloc(256);
			sprintf(channel_text, "Input :     ");
			gain_channel_text_box->label(channel_text);

			Fl_Box* gain_rgb_text_box = (Fl_Box*)gain_curve_info->child(1);
			char* rgb_text = (char*)malloc(256);
			sprintf(rgb_text, "Color :     ");
			gain_rgb_text_box->label(rgb_text);

			Fl_Box* gain_point_text_box = (Fl_Box*)gain_curve_info->child(2);
			char* point_text = (char*)malloc(256);
			sprintf(point_text, "Point :     ");
			gain_point_text_box->label(point_text);

			Fl_Group* gain_curve_points = (Fl_Group*)findChildByLabel(gain_curve_manual, "gain_curve_points");
			Fl_Group* point_round_button = (Fl_Group*)findChildByLabel(gain_curve_points, "point_round_button");
			Fl_Round_Button* btn_input_p1 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P1");
			Fl_Round_Button* btn_input_p2 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P2");
			Fl_Round_Button* btn_input_p3 = (Fl_Round_Button*)findChildByLabel(point_round_button, "P3");
			btn_input_p1->value(0);
			btn_input_p2->value(0);
			btn_input_p3->value(0);
			Fl_Group* p1 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point1");
			Fl_Float_Input* p1_x = (Fl_Float_Input*)findChildByLabel(p1, "point_x");
			Fl_Float_Input* p1_y = (Fl_Float_Input*)findChildByLabel(p1, "point_y");
			p1_x->value("");
			p1_y->value("");
			Fl_Group* p2 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point2");
			Fl_Float_Input* p2_x = (Fl_Float_Input*)findChildByLabel(p2, "point_x");
			Fl_Float_Input* p2_y = (Fl_Float_Input*)findChildByLabel(p2, "point_y");
			p2_x->value("");
			p2_y->value("");
			Fl_Group* p3 = (Fl_Group*)findChildByLabel(gain_curve_points, "gain_curve_point3");
			Fl_Float_Input* p3_x = (Fl_Float_Input*)findChildByLabel(p3, "point_x");
			Fl_Float_Input* p3_y = (Fl_Float_Input*)findChildByLabel(p3, "point_y");
			p3_x->value("");
			p3_y->value("");
			gain_curve_points->deactivate();

			Fl_Group* channel_round_button = (Fl_Group*)findChildByLabel(gain_curve_manual, "channel_round_button");
			Fl_Round_Button* btn_input1 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "1");
			Fl_Round_Button* btn_input2 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "2");
			Fl_Round_Button* btn_input3 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "3");
			Fl_Round_Button* btn_input4 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "4");
			Fl_Round_Button* btn_input5 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "5");
			Fl_Round_Button* btn_input6 = (Fl_Round_Button*)findChildByLabel(channel_round_button, "6");
			btn_input1->value(0);
			btn_input2->value(0);
			btn_input3->value(0);
			btn_input4->value(0);
			btn_input5->value(0);
			btn_input6->value(0);

			Fl_Group* rgb_round_button = (Fl_Group*)findChildByLabel(gain_curve_manual, "rgb_round_button");
			Fl_Round_Button* btn_input_r = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "R");
			Fl_Round_Button* btn_input_g = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "G");
			Fl_Round_Button* btn_input_b = (Fl_Round_Button*)findChildByLabel(rgb_round_button, "B");
			btn_input_r->value(0);
			btn_input_g->value(0);
			btn_input_b->value(0);
			rgb_round_button->deactivate();

			Fl_Button* btn_apply = (Fl_Button*)findChildByLabel(gain_curve_manual, "Apply");
			btn_apply->deactivate();
		}

		win->gain_curve_manual_win->show();
	}
	else if (btn->label() == "Auto Gain Off") {
		//QVS_Engine::QVS_setCallback_autoGainMode(onCallbackDone);
		//manager->setAutoGainMode(1);
		QVS_Engine::QVS_SetAutoGainMode(1);
		btn->label("Auto Gain On");
	}
	else if (btn->label() == "Auto Gain On") {
		//QVS_Engine::QVS_setCallback_autoGainMode(onCallbackDone);
		//manager->setAutoGainMode(0);
		QVS_Engine::QVS_SetAutoGainMode(0);
		btn->label("Auto Gain Off");
	}

	else if (btn->label() == "Nadir Off") {
		//manager->setNadirCompensationMode(NADIR_LOG_ON);
		QVS_Engine::QVS_SetNadirCompensationMode(NADIR_LOG_ON);
		btn->label("Nadir On");
	}
	else if (btn->label() == "Nadir On") {
		//manager->setNadirCompensationMode(NADIR_LOG_OFF);
		QVS_Engine::QVS_SetNadirCompensationMode(NADIR_LOG_OFF);
		btn->label("Nadir Off");
	}
	else if (btn->label() == "Nadir BOTTOM") {
		//manager->setNadirCompensationParam(NADIR_TOP, -1);
		QVS_Engine::QVS_SetNadirCompensationParam(NADIR_TOP, -1);
		btn->label("Nadir TOP");
	}
	else if (btn->label() == "Nadir TOP") {
		//manager->setNadirCompensationParam(NADIR_BOTTOM, -1);
		QVS_Engine::QVS_SetNadirCompensationParam(NADIR_BOTTOM, -1);
		btn->label("Nadir BOTTOM");
	}
	else if (btn->label() == "counter_size")
	{
		Fl_Simple_Counter* counter_size = (Fl_Simple_Counter*)btn;
		//manager->setNadirCompensationParam(-1, counter_size->value());
		QVS_Engine::QVS_SetNadirCompensationParam(-1, counter_size->value());
	}
	else if (btn->label() == "Browse...")
	{
		Fl_Native_File_Chooser* chooser = new Fl_Native_File_Chooser();
		chooser->title("Select nadir file");
		chooser->filter("PNG\t*.png");
		chooser->directory(".\\");
		//chooser->directory(manager->mNadirPath);
		chooser->directory(QVS_Engine::QVS_GetNadirPath());
		if (chooser->show() == 0) {
			//sprintf(manager->mNadirPath, "%s", chooser->filename());
			//manager->updateNadirImage();
			QVS_Engine::QVS_SetNadirPath(chooser->filename());
			QVS_Engine::QVS_UpdateNadirImage();
		}
	}
	else if (btn->label() == "@<")
	{
		if (btn->parent()->label() == "Video Delay")
		{
			//QVS_Engine::QVS_setCallback_videoBufferOffset(onCallbackDone);
			//int result = manager->setVideoBufferOffset(-1);
			int result = QVS_Engine::QVS_SetVideoBufferOffset(-1);
			char text_video_sync[256];
			float delay = -1.0f*result * (1.0f / 30.0f);
			if (result == 0)
			{
				delay = 0.00001f;
			}
			sprintf(text_video_sync, "%0.2f", delay); // 192000 = 1 sec
			Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
			Fl_Output* output_video_sync = (Fl_Output*)findChildByLabel(video, "sec");
			output_video_sync->value(text_video_sync);

			// set video delay (slow)
			// and set value of output layout
		}
	}
	else if (btn->label() == "@>")
	{
		if (btn->parent()->label() == "Video Delay")
		{
			//QVS_Engine::QVS_setCallback_videoBufferOffset(onCallbackDone);

			//int result = manager->setVideoBufferOffset(1);
			int result = QVS_Engine::QVS_SetVideoBufferOffset(1);
			char text_video_sync[256];
			float delay = -1.0f*result * (1.0f / 30.0f);
			if (result == 0)
			{
				delay = 0.00001f;
			}
			sprintf(text_video_sync, "%0.2f", delay); // 192000 = 1 sec
			Fl_Group* video = (Fl_Group*)findChildByLabel(subcontent, "Video Delay");
			Fl_Output* output_video_sync = (Fl_Output*)findChildByLabel(video, "sec");
			output_video_sync->value(text_video_sync);
		}
	}
}

int initMainLayout() {
	const char fltk[] = "fltk ";
	const char opengl[] = "opengl";
	const Fl_Menu_Item menuitems[] = {
		{ "&File", 0, 0, 0, FL_SUBMENU }, { "File &Selector . . .", FL_ALT + 's', (Fl_Callback *)cb_file_select },
		{ "Open &Dialog", FL_ALT + 'd', (Fl_Callback *)cb_open_dialog, 0, FL_MENU_DIVIDER },
		{ "&Fltk", FL_ALT + 'f', (Fl_Callback *)cb_pass, (void *)fltk },
		{ "&OpenGl", FL_ALT + 'o', (Fl_Callback *)cb_pass, (void *)opengl, FL_MENU_DIVIDER },
		{ "&Quit", FL_ALT + 'q', (Fl_Callback *)cb_quit },
		{ 0 },
		{ "&Help", 0, 0, 0, FL_SUBMENU },
		{ "&About", FL_ALT + 'a', (Fl_Callback *)cb_about },
		{ 0 },
		{ 0 }
	};

	int x = maincontent_x + 10;
	int y = maincontent_y + 10;
	int w = maincontent_w - 20;
	int h = maincontent_h - 20;

	Quram_Fl_Window *win = new Quram_Fl_Window(window_x, window_y, window_w, window_h, "VentaVRLive");
	{
		Fl_Menu_Bar *menu = new Fl_Menu_Bar(menubar_x, menubar_y, window_w, menubar_h);
		{
			menu->menu(menuitems);
			//menu->add("File/Quit", FL_CTRL + 'q', Quit_CB);
			menu->add("&Edit/&Change", FL_CTRL + 'c', Change_CB);
			menu->add("&Edit/Submenu/&Aaa");
			menu->add("&Edit/Submenu/&Bbb");
			menu->color(FL_DARK2);
		}

		Fl_Group *subjectbar = new Fl_Group(subjectbar_x, subjectbar_y, subjectbar_w, subjectbar_h, "subjectbar");
		{
			Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, subjectbar_x, subjectbar_y, subjectbar_w, subjectbar_h, "");
			{
				backgournd->color(FL_DARK2);
			}

			Fl_Box *margin = new Fl_Box(FL_FLAT_BOX, subject_left_box_x + subject_left_box_w, subjectbar_y, subjectbar_w - subject_left_box_w - time_right_box_w, subjectbar_h, "");
			{
				margin->color(FL_DARK2);
			}

			Fl_Box *box_subject = new Fl_Box(subject_left_box_x, subject_left_box_y, subject_left_box_w, subject_left_box_h, "");
			{
				box_subject->align(FL_ALIGN_RIGHT);
				box_subject->labelsize(30);
			}

			Fl_Box *box_time_fps = new Fl_Box(time_right_box_x, time_right_box_y, time_right_box_w, time_right_box_h, "");
			{
				box_time_fps->align(FL_ALIGN_LEFT);
			}

			subjectbar->end();
			subjectbar->resizable(margin);

			subjectbar->labeltype(FL_NO_LABEL);
		}

		static Fl_Group *content = new Fl_Group(content_x, content_y, content_w, content_h, "content");
		{
			Fl_Group *maincontent = new Fl_Group(maincontent_x, maincontent_y, maincontent_w, maincontent_h, "maincontent");
			{
				Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, maincontent_x, maincontent_y, maincontent_w, maincontent_h, "");
				{
					backgournd->color(FL_DARK1);
				}

				Fl_Box *textBox = new Fl_Box(FL_FLAT_BOX, maincontent_x, maincontent_y, maincontent_w, default_h, "Select a configuration section");
				{
					//textBox->color(FL_DARK1);
				}

				Fl_Button* btn_maincontent_1 = new Fl_Button(maincontent_x + btn_gap, maincontent_y + textBox->h() + btn_gap, btn_w, btn_h, "New\n\nConfiguration");
				btn_maincontent_1->callback(configuration_btn_callback, win);

				Fl_Button* btn_maincontent_2 = new Fl_Button(btn_maincontent_1->x() + btn_w + btn_gap, maincontent_y + textBox->h() + btn_gap, btn_w, btn_h, "Load\n\nConfiguration");
				btn_maincontent_2->callback(configuration_btn_callback, win);

				Fl_Button* btn_maincontent_3 = new Fl_Button(btn_maincontent_2->x() + btn_w + btn_gap, maincontent_y + textBox->h() + btn_gap, btn_w, btn_h);
				btn_maincontent_3->callback(configuration_btn_callback, win);
				btn_maincontent_3->hide();

				maincontent->add(btn_maincontent_1);
				maincontent->add(btn_maincontent_2);
				maincontent->add(btn_maincontent_3);

				maincontent->end();
				win->gl_main_win_group = maincontent;

				maincontent->labeltype(FL_NO_LABEL);
			}

			Fl_Group *submenu = new Fl_Group(submenu_x, submenu_y, submenu_w, submenu_h, "submenu");
			{
				Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, submenu_x, submenu_y, submenu_w, submenu_h, "");
				{
					backgournd->color(FL_DARK2);
				}

				Fl_Button* btn_submenu_1 = new Fl_Button(submenu_x + menu_btn_gap, submenu_y + menu_btn_gap, menu_btn_w, menu_btn_h);
				btn_submenu_1->callback(submenu_btn_callback, win);
				btn_submenu_1->hide();

				Fl_Button* btn_submenu_2 = new Fl_Button(submenu_x + menu_btn_gap, btn_submenu_1->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h);
				btn_submenu_2->callback(submenu_btn_callback, win);
				btn_submenu_2->hide();

				Fl_Button* btn_submenu_3 = new Fl_Button(submenu_x + menu_btn_gap, btn_submenu_2->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h);
				btn_submenu_3->callback(submenu_btn_callback, win);
				btn_submenu_3->hide();

				Fl_Button* btn_submenu_4 = new Fl_Button(submenu_x + menu_btn_gap, btn_submenu_3->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h);
				btn_submenu_4->callback(submenu_btn_callback, win);
				btn_submenu_4->hide();

				Fl_Box *margin = new Fl_Box(FL_FLAT_BOX, submenu_x, btn_submenu_4->y() + menu_btn_h + menu_btn_gap, submenu_w, window_h - (btn_submenu_4->y() + menu_btn_h + menu_btn_gap), "");
				{
					margin->color(FL_DARK2);
				}

				submenu->add(btn_submenu_1);
				submenu->add(btn_submenu_2);
				submenu->add(btn_submenu_3);
				submenu->add(btn_submenu_4);

				submenu->end();
				submenu->resizable(margin);

				submenu->labeltype(FL_NO_LABEL);
			}

			Fl_Group *subcontent = new Fl_Group(subcontent_x, subcontent_y, subcontent_w, subcontent_h, "subcontent");
			{
				Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, subcontent_x, subcontent_y, subcontent_w, subcontent_h, "");
				{
					backgournd->color(FL_DARK2);
				}

				Fl_Button* btn_output = new Fl_Button(subcontent_x + (subcontent_w / 2) - subcontent_btn_w - (menu_btn_gap / 2) + menu_btn_gap, subcontent_y + menu_output_btn_gap, subcontent_btn_w, subcontent_output_btn_h, "OUTPUT OFF");
				btn_output->callback(subcontent_btn_callback, win);
				btn_output->deactivate();
				btn_output->show();

				Fl_Button* btn_blend = new Fl_Button(subcontent_x + menu_btn_gap, subcontent_y + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Multiband Blending");
				btn_blend->labelsize(11);
				btn_blend->callback(subcontent_btn_callback, win);
				btn_blend->deactivate();
				btn_blend->show();

				Fl_Button* btn_seam = new Fl_Button(btn_blend->x() + subcontent_btn_w + menu_btn_gap, subcontent_y + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Seam Dist");
				btn_seam->callback(subcontent_btn_callback, win);
				btn_seam->deactivate();
				btn_seam->show();

				Fl_Button* btn_color_setting = new Fl_Button(btn_seam->x() + subcontent_btn_w + menu_btn_gap, subcontent_y + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Color Setting");
				btn_color_setting->callback(subcontent_btn_callback, win);
				btn_color_setting->deactivate();
				btn_color_setting->show();

				Fl_Button* btn_auto_gain = new Fl_Button(btn_blend->x() + subcontent_btn_w + menu_btn_gap, btn_blend->y() + btn_blend->h() + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Auto Gain Off");
				btn_auto_gain->callback(subcontent_btn_callback, win);
				btn_auto_gain->deactivate();
				btn_auto_gain->show();

				Fl_Choice* choice_gain = new Fl_Choice(subcontent_x + menu_btn_gap, btn_blend->y() + btn_blend->h() + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "choice_gain");
				{
					choice_gain->add("Gain Off");
					choice_gain->add("Gain Average");
					choice_gain->add("Gain Color");
					choice_gain->add("Gain Curve Manual");
					choice_gain->add("Gain Curve Auto Color");
					//choice_gain->add("Gain Curve Auto Average");
					

					//choice_gain->add("Gain Curve Manual Average");
					

					//choice_gain->add("Gain AverageFull");
					//choice_gain->add("Gain ColorFull");
					//choice_gain->add("Gain YUVFull");
					//choice_gain->add("Gain AverageHalf");
					//choice_gain->add("Gain ColorHalf");
					//choice_gain->add("Gain YUVHalf");
					choice_gain->value(0);
					choice_gain->callback(subcontent_btn_callback, win);
					choice_gain->labeltype(FL_NO_LABEL);
					choice_gain->deactivate();
				}

				Fl_Button* btn_curve_setting = new Fl_Button(choice_gain->x() + subcontent_btn_w + menu_btn_gap + subcontent_btn_w + menu_btn_gap, btn_blend->y() + btn_blend->h() + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Curve Setting");
				btn_curve_setting->callback(subcontent_btn_callback, win);
				btn_curve_setting->deactivate();
				btn_curve_setting->show();

				Fl_Button* btn_up = new Fl_Button(subcontent_x + maincontent_w - subcontent_btn_w - menu_btn_gap, subcontent_y + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Nadir BOTTOM");
				btn_up->callback(subcontent_btn_callback, win);
				btn_up->deactivate();
				btn_up->show();

				Fl_Button* btn_on = new Fl_Button(btn_up->x() - subcontent_btn_w - menu_btn_gap, subcontent_y + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Nadir Off");
				btn_on->callback(subcontent_btn_callback, win);
				btn_on->deactivate();
				btn_on->show();

				Fl_Button* btn_browse = new Fl_Button(subcontent_x + maincontent_w - subcontent_btn_w - menu_btn_gap, btn_up->y() + subcontent_btn_h + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Browse...");
				btn_browse->callback(subcontent_btn_callback, win);
				btn_browse->deactivate();
				btn_browse->show();

				Fl_Simple_Counter* counter_size = new Fl_Simple_Counter(btn_browse->x() - subcontent_btn_w - menu_btn_gap, btn_up->y() + subcontent_btn_h + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "counter_size");
				counter_size->bounds(1, 50);
				counter_size->step(1);
				counter_size->value(17);
				counter_size->callback(subcontent_btn_callback, win);
				counter_size->deactivate();
				counter_size->show();
				counter_size->labeltype(FL_NO_LABEL);

				Fl_Group *video = new Fl_Group(btn_on->x() - subcontent_btn_w - menu_btn_gap - menu_btn_gap, subcontent_y + menu_btn_gap + subcontent_small_btn_h + menu_btn_gap, subcontent_btn_w, subcontent_btn_h, "Video Delay");
				{
					Fl_Button* btn_left = new Fl_Button(btn_on->x() - subcontent_btn_w - menu_btn_gap, subcontent_y + menu_btn_gap + subcontent_small_btn_h + menu_btn_gap, subcontent_small_btn_w, subcontent_small_btn_h, "@<");
					btn_left->callback(subcontent_btn_callback, win);
					btn_left->deactivate();
					btn_left->show();

					Fl_Button* btn_right = new Fl_Button(btn_left->x() + subcontent_small_btn_w + menu_btn_gap, btn_left->y(), subcontent_small_btn_w, subcontent_small_btn_h, "@>");
					btn_right->callback(subcontent_btn_callback, win);
					btn_right->deactivate();
					btn_right->show();

					Fl_Output* output_video_sync = new Fl_Output(btn_right->x() + subcontent_small_btn_w + menu_btn_gap, btn_left->y(), subcontent_middle_btn_w, subcontent_middle_btn_h, "sec");
					output_video_sync->align(FL_ALIGN_RIGHT);
					output_video_sync->color(FL_DARK2);
					output_video_sync->value("0.00");
					output_video_sync->deactivate();
					output_video_sync->show();
					output_video_sync->labeltype(FL_NO_LABEL);

					video->end();
					video->align(FL_ALIGN_LEFT);
					video->deactivate();
					//video->labeltype(FL_NO_LABEL);
				}

				subcontent->end();

				subcontent->labeltype(FL_NO_LABEL);
			}

			content->add(maincontent);
			content->add(submenu);
			content->add(subcontent);

			content->end();
			content->resizable(maincontent);

			content->labeltype(FL_NO_LABEL);
		}

		Fl_Group *mainmenu = new Fl_Group(mainmenu_x, mainmenu_y, mainmenu_w, mainmenu_h, "mainmenu");
		{
			Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, mainmenu_x, mainmenu_y, mainmenu_w, mainmenu_h, "");
			{
				backgournd->color(FL_DARK2);
			}

			Fl_Button* btn_sources = new Fl_Button(mainmenu_x + menu_btn_gap, mainmenu_y + menu_btn_gap, menu_btn_w, menu_btn_h, "Sources");
			btn_sources->callback(mainmenu_btn_callback, win);
			btn_sources->hide();

			Fl_Button* btn_panorama = new Fl_Button(mainmenu_x + menu_btn_gap, btn_sources->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h, "Panorama");
			btn_panorama->callback(mainmenu_btn_callback, win);
			btn_panorama->hide();

			Fl_Button* btn_interactive = new Fl_Button(mainmenu_x + menu_btn_gap, btn_panorama->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h, "Interactive");
			btn_interactive->callback(mainmenu_btn_callback, win);
			btn_interactive->hide();

			Fl_Button* btn_configuration = new Fl_Button(mainmenu_x + menu_btn_gap, btn_interactive->y() + menu_btn_h + menu_btn_gap, menu_btn_w, menu_btn_h, "Configuration");
			btn_configuration->callback(mainmenu_btn_callback, win);
			btn_configuration->hide();

			Fl_Box *margin = new Fl_Box(FL_FLAT_BOX, mainmenu_x, btn_configuration->y() + menu_btn_h + menu_btn_gap, mainmenu_w, window_h - (btn_configuration->y() + menu_btn_h + menu_btn_gap), "");
			{
				margin->color(FL_DARK2);
			}

			mainmenu->end();
			mainmenu->resizable(margin);

			mainmenu->labeltype(FL_NO_LABEL);
		}

		GLWin* glwin = new GLWin(x, y, w, h);
		win->gl_main_win = glwin;
		win->gl_main_win->resize(0, 0, 0, 0);

		win->add(menu);
		win->add(subjectbar);
		win->add(content);
		win->add(mainmenu);

		win->end();
		win->size_range(screenw * 0.5729166666666667, screenh * 0.5);
		win->resizable(content);
		win->show();

		Fl::repeat_timeout(1, Quram_Fl_Window::timer_cb, win);
	}

	win->callback(exit_callback, win);

	bool initialized = QVS_Engine::QVS_InitEngineManager();
	if (initialized)
	{
		printf("QVR Engine Init Success!!\n");
	}
	else
	{
		printf("QVR Engine Init Failed!!\n");
		return QVS_FAIL;
	}
	initOutputsLayout(win);
	initPanoramaLayout(win);
	initCalibrationLayout(win);
	initEditInputLayout(win);
	initDoCalibrationLayout(win);
	initNewConfigurationLayout(win);
	initColorSettingLayout(win);
	initGainCurveManualLayout(win);

	Fl::add_idle(idle, win->gl_main_win);

	return(Fl::run());
}

void initOutputsLayout(Quram_Fl_Window* win) {
	int outputs_window_x = win->x() + (win->w() / 2) - (outputs_window_w / 2);
	int outputs_window_y = win->y() + (win->h() / 2) - (outputs_window_h / 2);

	win->outputs_win = new Fl_Window(outputs_window_x, outputs_window_y, outputs_window_w, outputs_window_h, "Outputs");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, outputs_window_w, outputs_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *bmoutput_settings = new Fl_Group(bmoutput_settings_x, bmoutput_settings_y, bmoutput_settings_w, bmoutput_settings_h, "bmoutput_settings");
		{
			Fl_Box *bmoutput_text_box = new Fl_Box(FL_FLAT_BOX, bmoutput_settings_x, bmoutput_settings_y + outputs_margin_h, outputs_margin_w, outputs_btn_h, "Black Magic Output settings --------------------------------------------------------------------------------------------------");
			{
				bmoutput_text_box->align(FL_ALIGN_RIGHT);
				bmoutput_text_box->color(FL_DARK2);
			}

			Fl_Box *device_text_box = new Fl_Box(FL_FLAT_BOX, bmoutput_settings_x, bmoutput_text_box->y() + bmoutput_text_box->h() + outputs_margin_h, outputs_margin_w, outputs_btn_h, "Device");
			{
				device_text_box->align(FL_ALIGN_RIGHT);
				device_text_box->color(FL_DARK2);
			}

			Fl_Choice *device_choice = new Fl_Choice(outputs_btn_w + outputs_margin_w + outputs_margin_w, bmoutput_text_box->y() + bmoutput_text_box->h() + outputs_margin_h, outputs_long_btn_w, outputs_btn_h, "device_choice");
			{
				device_choice->labeltype(FL_NO_LABEL);
			}

			Fl_Box *display_text_box = new Fl_Box(FL_FLAT_BOX, bmoutput_settings_x, device_text_box->y() + device_text_box->h() + outputs_margin_h, outputs_margin_w, outputs_btn_h, "Display");
			{
				display_text_box->align(FL_ALIGN_RIGHT);
				display_text_box->color(FL_DARK2);
			}

			Fl_Choice *display_choice = new Fl_Choice(outputs_btn_w + outputs_margin_w + outputs_margin_w, device_text_box->y() + device_text_box->h() + outputs_margin_h, outputs_long_btn_w, outputs_btn_h, "display_choice");
			{
				display_choice->labeltype(FL_NO_LABEL);
				display_choice->deactivate();
			}

			bmoutput_settings->end();

			bmoutput_settings->labeltype(FL_NO_LABEL);
		}

		Fl_Button *save_btn = new Fl_Button(outputs_window_w - outputs_btn_w - outputs_margin_w, bmoutput_settings->y() + bmoutput_settings->h() + outputs_long_margin_h, outputs_btn_w, outputs_btn_h, "Save");
		save_btn->callback(outputs_btn_callback, win);
	}

	win->outputs_win->end();
}

void initPanoramaLayout(Quram_Fl_Window* win) {
	int panorama_window_x = win->x() + (win->w() / 2) - (panorama_window_w / 2);
	int panorama_window_y = win->y() + (win->h() / 2) - (panorama_window_h / 2);

	win->panorama_win = new Fl_Window(panorama_window_x, panorama_window_y, panorama_window_w, panorama_window_h, "Panorama");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, panorama_window_w, panorama_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *panorama_settings = new Fl_Group(panorama_settings_x, panorama_settings_y, panorama_settings_w, panorama_settings_h, "panorama_settings");
		{
			Fl_Box *width_text_box = new Fl_Box(FL_FLAT_BOX, panorama_settings->x(), panorama_settings->y() + panorama_margin_h, panorama_margin_w, panorama_btn_h, "Panorama Size");
			{
				width_text_box->align(FL_ALIGN_RIGHT);
				width_text_box->color(FL_DARK2);
			}

			Fl_Choice *size_choice = new Fl_Choice(panorama_btn_w + panorama_margin_w + panorama_margin_w, panorama_settings->y() + panorama_margin_h, panorama_btn_w, panorama_btn_h, "size_choice");
			{
				size_choice->labeltype(FL_NO_LABEL);
				size_choice->add("3840x1920");
				size_choice->add("3840x2160");
			}

			Fl_Button *save_btn = new Fl_Button(panorama_window_w - panorama_btn_w - panorama_margin_w, width_text_box->y() + width_text_box->h() + panorama_long_margin_h, panorama_btn_w, panorama_btn_h, "Save");
			{
				save_btn->callback(panorama_btn_callback, win);
			}

			panorama_settings->end();

			panorama_settings->labeltype(FL_NO_LABEL);
		}
	}
	win->panorama_win->end();
}

void initCalibrationLayout(Quram_Fl_Window* win) {
	int calibration_window_x = win->x() + (win->w() / 2) - (calibration_window_w / 2);
	int calibration_window_y = win->y() + (win->h() / 2) - (calibration_window_h / 2);

	win->calibration_win = new Fl_Window(calibration_window_x, calibration_window_y, calibration_window_w, calibration_window_h, "Calibration");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, calibration_window_w, calibration_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *new_calibration = new Fl_Group(new_calibration_x, new_calibration_y, new_calibration_w, new_calibration_h, "new_calibration");
		{
			Fl_Box *new_text_box = new Fl_Box(FL_FLAT_BOX, new_calibration_x, new_calibration_y + calibration_margin_h, calibration_margin_w, calibration_btn_h, "New calibration ------------------------------------------------------------------------------------------------------------------");
			{
				new_text_box->align(FL_ALIGN_RIGHT);
				new_text_box->color(FL_DARK2);
			}

			Fl_Box *projection_text_box = new Fl_Box(FL_FLAT_BOX, 0, new_text_box->y() + new_text_box->h() + calibration_margin_h, calibration_margin_w, calibration_btn_h, "Projection");
			{
				projection_text_box->align(FL_ALIGN_RIGHT);
				projection_text_box->color(FL_DARK2);
				projection_text_box->deactivate();
			}

			Fl_Choice *projection_choice = new Fl_Choice(calibration_btn_w + calibration_margin_w + calibration_margin_w, new_text_box->y() + new_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h);
			{
				projection_choice->add("Rectilinear");
				projection_choice->add("Circular fisheye");
				projection_choice->add("Fullframe fisheye");
				projection_choice->value(2);
				projection_choice->deactivate();
			}

			Fl_Box *horizontalfov_text_box = new Fl_Box(projection_choice->x() + projection_choice->w(), new_text_box->y() + new_text_box->h() + calibration_margin_h, calibration_long_margin_w, calibration_btn_h, "Horizontal FOV");
			{
				horizontalfov_text_box->align(FL_ALIGN_RIGHT);
				horizontalfov_text_box->color(FL_DARK2);
				horizontalfov_text_box->deactivate();
			}

			Fl_Counter *horizontalfov_counter = new Fl_Counter(horizontalfov_text_box->x() + calibration_btn_w + calibration_long_margin_w + calibration_margin_w, new_text_box->y() + new_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h);
			horizontalfov_counter->value(120.0f);
			horizontalfov_counter->deactivate();

			Fl_Button *create_btn = new Fl_Button(calibration_margin_w, projection_text_box->y() + projection_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h, "Create");
			create_btn->callback(calibration_btn_callback, win);
			create_btn->deactivate();

			new_calibration->end();

			new_calibration->labeltype(FL_NO_LABEL);
		}

		Fl_Group *import_calibration = new Fl_Group(import_calibration_x, import_calibration_y, import_calibration_w, import_calibration_h, "import_calibration");
		{
			Fl_Box *import_text_box = new Fl_Box(FL_FLAT_BOX, import_calibration_x, import_calibration_y + calibration_margin_h, calibration_margin_w, calibration_btn_h, "Import calibration ---------------------------------------------------------------------------------------------------------------");
			{
				import_text_box->align(FL_ALIGN_RIGHT);
				import_text_box->color(FL_DARK2);
			}

			Fl_Box *template_text_box = new Fl_Box(FL_FLAT_BOX, 0, import_text_box->y() + import_text_box->h() + calibration_margin_h, calibration_margin_w, calibration_btn_h, "Template");
			{
				template_text_box->align(FL_ALIGN_RIGHT);
				template_text_box->color(FL_DARK2);
			}

			Fl_Input *template_input = new Fl_Input(calibration_btn_w + calibration_margin_w + calibration_margin_w, import_text_box->y() + import_text_box->h() + calibration_margin_h, template_input_w, template_input_h, "template_input");
			{
				template_input->labeltype(FL_NO_LABEL);
			}

			Fl_Button *template_browse_btn = new Fl_Button(template_input->x() + template_input->w() + calibration_margin_w, import_text_box->y() + import_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h, "Browse...");
			template_browse_btn->callback(calibration_btn_callback, win);

			Fl_Button *apply_btn = new Fl_Button(calibration_margin_w, template_text_box->y() + template_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h, "Apply");
			apply_btn->callback(calibration_btn_callback, win);

			import_calibration->add(template_input);

			import_calibration->end();

			import_calibration->labeltype(FL_NO_LABEL);
		}

		Fl_Group *setting_calibration = new Fl_Group(setting_calibration_x, setting_calibration_y, setting_calibration_w, setting_calibration_h, "setting_calibration");
		{
			Fl_Box *setting_text_box = new Fl_Box(FL_FLAT_BOX, setting_calibration_x, setting_calibration_y + calibration_margin_h, calibration_margin_w, calibration_btn_h, "Settings --------------------------------------------------------------------------------------------------------------------------");
			{
				setting_text_box->align(FL_ALIGN_RIGHT);
				setting_text_box->color(FL_DARK2);
			}

			Fl_Box *snapshot_text_box = new Fl_Box(FL_FLAT_BOX, 0, setting_text_box->y() + setting_text_box->h() + calibration_margin_h, calibration_margin_w, calibration_btn_h, "Save source video snapshots to:");
			{
				snapshot_text_box->align(FL_ALIGN_RIGHT);
				snapshot_text_box->color(FL_DARK2);
			}

			Fl_Input *snapshot_input = new Fl_Input(calibration_margin_w, snapshot_text_box->y() + snapshot_text_box->h() + calibration_margin_h, snapshot_input_w, snapshot_input_h, "snapshot_input");
			{
				//snapshot_input->value(win->gl_main_win->engineManager->snapshot_path);

				snapshot_input->deactivate();
				snapshot_input->labeltype(FL_NO_LABEL);
			}

			Fl_Button *snapshot_browse_btn = new Fl_Button(snapshot_input->x() + snapshot_input->w() + calibration_margin_w, snapshot_text_box->y() + snapshot_text_box->h() + calibration_margin_h, calibration_btn_w, calibration_btn_h, "Browse...");
			snapshot_browse_btn->callback(calibration_btn_callback, win);

			setting_calibration->add(snapshot_input);

			setting_calibration->end();

			setting_calibration->labeltype(FL_NO_LABEL);
		}

		win->calibration_win->add(new_calibration);
		win->calibration_win->add(import_calibration);
		win->calibration_win->add(setting_calibration);
	}
	win->calibration_win->end();
}

void initEditInputLayout(Quram_Fl_Window* win) {
	int edit_input_window_x = win->x() + (win->w() / 2) - (edit_input_window_w / 2);
	int edit_input_window_y = win->y() + (win->h() / 2) - (edit_input_window_h / 2);

	win->edit_input_win = new Fl_Window(edit_input_window_x, edit_input_window_y, edit_input_window_w, edit_input_window_h, "Edit Input");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, edit_input_window_w, edit_input_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *capture_config = new Fl_Group(capture_config_x, capture_config_y, capture_config_w, capture_config_h, "capture_config");
		{
			Fl_Box *capture_config_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, capture_config_y + edit_input_margin_h, edit_input_margin_w, edit_input_btn_h, "Capture configuration -----------------------------------------------------------------------------------------------");
			{
				capture_config_text_box->align(FL_ALIGN_RIGHT);
				capture_config_text_box->color(FL_DARK2);
			}

			Fl_Choice *camera_index_choice[CAMERA_MAX];
			for (int i = 0; i < CAMERA_MAX; i++) {
				camera_index_choice[i] = new Fl_Choice(edit_input_long_margin_w + (edit_input_btn_w + edit_input_long_margin_w) * (i % 3), capture_config_text_box->y() + capture_config_text_box->h() + edit_input_long_margin_h + (edit_input_btn_h + edit_input_long_margin_h) * (i / 3), edit_input_btn_w, edit_input_btn_h);
				{
					sprintf(camera_index_choice_label[i], "Camera Input %d", i);
					camera_index_choice[i]->label(camera_index_choice_label[i]);

					camera_index_choice[i]->callback(edit_input_choice_callback, win);

					camera_index_choice[i]->align(FL_ALIGN_TOP_LEFT);
					camera_index_choice[i]->hide();
				}
			}

			capture_config->end();

			capture_config->labeltype(FL_NO_LABEL);
		}

		Fl_Group *input_parameter_config = new Fl_Group(input_parameter_config_x, input_parameter_config_y, input_parameter_config_w, input_parameter_config_h, "input_parameter_config");
		{
			Fl_Box *input_parameter_config_text_box = new Fl_Box(FL_FLAT_BOX, input_parameter_config_x, input_parameter_config_y + edit_input_margin_h, edit_input_margin_w, edit_input_btn_h, "Configure input parameters ----------------------------------------------------------------------------------------");
			{
				input_parameter_config_text_box->align(FL_ALIGN_RIGHT);
				input_parameter_config_text_box->color(FL_DARK2);
			}

			Fl_Box *fps_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, input_parameter_config_text_box->y() + input_parameter_config_text_box->h() + edit_input_margin_h, edit_input_margin_w, edit_input_btn_h, "FPS");
			{
				fps_text_box->align(FL_ALIGN_RIGHT);
				fps_text_box->color(FL_DARK2);
				//fps_text_box->deactivate();
			}

			Fl_Float_Input *fps_input = new Fl_Float_Input(edit_input_text_w + edit_input_margin_w + edit_input_margin_w, input_parameter_config_text_box->y() + input_parameter_config_text_box->h() + edit_input_margin_h, edit_input_btn_w, edit_input_btn_h, "fps_input");
			{
				fps_input->labeltype(FL_NO_LABEL);
				//fps_input->deactivate();
			}

			Fl_Box *width_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, fps_text_box->y() + fps_text_box->h() + edit_input_margin_h, edit_input_margin_w, edit_input_btn_h, "Width");
			{
				width_text_box->align(FL_ALIGN_RIGHT);
				width_text_box->color(FL_DARK2);
				width_text_box->deactivate();
			}

			Fl_Int_Input *width_input = new Fl_Int_Input(edit_input_text_w + edit_input_margin_w + edit_input_margin_w, fps_text_box->y() + fps_text_box->h() + edit_input_margin_h, edit_input_btn_w, edit_input_btn_h, "width_input");
			{
				width_input->labeltype(FL_NO_LABEL);
				width_input->deactivate();
			}

			Fl_Box *height_text_box = new Fl_Box(FL_FLAT_BOX, width_input->x() + width_input->w(), fps_text_box->y() + fps_text_box->h() + edit_input_margin_h, edit_input_long_margin_w, edit_input_btn_h, "Height");
			{
				height_text_box->align(FL_ALIGN_RIGHT);
				height_text_box->color(FL_DARK2);
				height_text_box->deactivate();
			}

			Fl_Int_Input *height_input = new Fl_Int_Input(height_text_box->x() + edit_input_text_w + edit_input_long_margin_w + edit_input_margin_w, fps_text_box->y() + fps_text_box->h() + edit_input_margin_h, edit_input_btn_w, edit_input_btn_h, "height_input");
			{
				height_input->labeltype(FL_NO_LABEL);
				height_input->deactivate();
			}

			Fl_Box *lens_type_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, width_input->y() + width_input->h() + edit_input_margin_h, edit_input_margin_w, edit_input_btn_h, "Lens type");
			{
				lens_type_text_box->align(FL_ALIGN_RIGHT);
				lens_type_text_box->color(FL_DARK2);
				//width_text_box->deactivate();
			}

			Fl_Choice* choice_lens = new Fl_Choice(edit_input_text_w + edit_input_margin_w + edit_input_margin_w, width_input->y() + width_input->h() + edit_input_margin_h, edit_input_btn_w, edit_input_btn_h, "choice_lens");
			{
				choice_lens->add("Rectilinear");
				choice_lens->add("Cylindrical panorama");
				choice_lens->add("Circular Fisheye");
				choice_lens->add("Full Frame Fisheye");
				//choice_lens->add("Cylindrical panorama");
				//choice_lens->add("Equirectangular panorama");

				choice_lens->value(3);
				choice_lens->callback(subcontent_btn_callback, win);
				choice_lens->labeltype(FL_NO_LABEL);
				//choice_lens->deactivate();
			}

			Fl_Box *fov_text_box = new Fl_Box(FL_FLAT_BOX, choice_lens->x() + choice_lens->w(), height_input->y() + height_input->h() + edit_input_margin_h, edit_input_long_margin_w, edit_input_btn_h, "FOV");
			{
				fov_text_box->align(FL_ALIGN_RIGHT);
				fov_text_box->color(FL_DARK2);
				//height_text_box->deactivate();
			}

			Fl_Int_Input *fov_input = new Fl_Int_Input(fov_text_box->x() + edit_input_text_w + edit_input_long_margin_w + edit_input_margin_w, height_text_box->y() + height_text_box->h() + edit_input_margin_h, edit_input_btn_w, edit_input_btn_h, "fov_input");
			{
				fov_input->labeltype(FL_NO_LABEL);
			}

			Fl_Button *save_btn = new Fl_Button(input_parameter_config_w - edit_input_btn_w - edit_input_margin_w, width_text_box->y() + width_text_box->h() + edit_input_long_margin_h, edit_input_btn_w, edit_input_btn_h, "Save");
			save_btn->callback(edit_input_btn_callback, win);

			input_parameter_config->end();

			input_parameter_config->labeltype(FL_NO_LABEL);
		}
	}
	win->edit_input_win->end();
}

void initDoCalibrationLayout(Quram_Fl_Window* win) {
	int do_calibration_window_x = win->x() + (win->w() / 2) - (do_calibration_window_w / 2);
	int do_calibration_window_y = win->y() + (win->h() / 2) - (do_calibration_window_h / 2);

	win->do_calibration_win = new Fl_Window(do_calibration_window_x, do_calibration_window_y, do_calibration_window_w, do_calibration_window_h, "Do Calibration");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, do_calibration_window_w, do_calibration_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *do_calibration = new Fl_Group(do_calibration_x, do_calibration_y, do_calibration_w, do_calibration_h, "do_calibration");
		{
			Fl_Group *round_button = new Fl_Group(do_calibration->x(), do_calibration->y(), do_calibration_round_button_w, do_calibration_round_button_h, "round_button");
			{
				Fl_Round_Button *image_round_button[CAMERA_MAX];
				for (int i = 0; i < CAMERA_MAX; i++) {
					image_round_button[i] = new Fl_Round_Button(do_calibration_margin_w + (do_calibration_round_btn_w + do_calibration_margin_w) * i, round_button->y() + do_calibration_margin_h, do_calibration_round_btn_w, do_calibration_round_btn_h);
					{
						sprintf(image_round_button_label[i], "%d", i);
						image_round_button[i]->label(image_round_button_label[i]);

						image_round_button[i]->down_box(FL_FRAME_BOX);
						image_round_button[i]->hide();
					}
				}

				round_button->end();

				round_button->labeltype(FL_NO_LABEL);
			}

			Fl_Button *apply_btn = new Fl_Button(do_calibration_window_w - do_calibration_btn_w - do_calibration_margin_w, round_button->y() + round_button->h(), do_calibration_btn_w, do_calibration_btn_h, "Apply");
			apply_btn->callback(do_calibration_btn_callback, win);

			do_calibration->end();

			do_calibration->labeltype(FL_NO_LABEL);
		}
	}
	win->do_calibration_win->end();
}

void initNewConfigurationLayout(Quram_Fl_Window* win) {
	int new_configuration_window_x = win->x() + (win->w() / 2) - (new_configuration_window_w / 2);
	int new_configuration_window_y = win->y() + (win->h() / 2) - (new_configuration_window_h / 2);

	win->new_configuration_win = new Fl_Window(new_configuration_window_x, new_configuration_window_y, new_configuration_window_w, new_configuration_window_h, "New Configuration");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, new_configuration_window_w, new_configuration_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *new_configuration = new Fl_Group(new_configuration_x, new_configuration_y, new_configuration_w, new_configuration_h, "new_configuration");
		{
			Fl_Box *capture_type_text_box = new Fl_Box(FL_FLAT_BOX, new_configuration_x, new_configuration_y + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Video capture type");
			{
				capture_type_text_box->align(FL_ALIGN_RIGHT);
				capture_type_text_box->color(FL_DARK2);

				capture_type_text_box->hide();
			}

			Fl_Choice *capture_type_choice = new Fl_Choice(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, new_configuration_y + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "capture_type_choice");
			{
				capture_type_choice->add("Magewell");
				capture_type_choice->add("Blackmagic");
				capture_type_choice->add("Magewell USB");
				capture_type_choice->value(0);
				capture_type_choice->labeltype(FL_NO_LABEL);

				capture_type_choice->hide();
			}

			Fl_Box *number_of_camera_text_box = new Fl_Box(FL_FLAT_BOX, new_configuration_x, capture_type_text_box->y() + capture_type_text_box->h() + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Number of Camera");
			{
				number_of_camera_text_box->align(FL_ALIGN_RIGHT);
				number_of_camera_text_box->color(FL_DARK2);
			}

			Fl_Int_Input *number_of_camera_input = new Fl_Int_Input(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, capture_type_text_box->y() + capture_type_text_box->h() + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "number_of_camera_input");
			{
				number_of_camera_input->labeltype(FL_NO_LABEL);
				number_of_camera_input->value("2");
			}

			Fl_Box *width_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, number_of_camera_text_box->y() + number_of_camera_text_box->h() + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Width");
			{
				width_text_box->align(FL_ALIGN_RIGHT);
				width_text_box->color(FL_DARK2);
			}

			Fl_Int_Input *width_input = new Fl_Int_Input(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, number_of_camera_text_box->y() + number_of_camera_text_box->h() + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "width_input");
			{
				width_input->labeltype(FL_NO_LABEL);
				width_input->value("3840");
			}

			Fl_Box *height_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, width_text_box->y() + width_text_box->h() + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Height");
			{
				height_text_box->align(FL_ALIGN_RIGHT);
				height_text_box->color(FL_DARK2);
			}

			Fl_Int_Input *height_input = new Fl_Int_Input(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, width_text_box->y() + width_text_box->h() + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "height_input");
			{
				height_input->labeltype(FL_NO_LABEL);
				height_input->value("2160");
			}

			//added output size 
			Fl_Box *output_width_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, height_text_box->y() + height_text_box->h() + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Output_Width");
			{
				output_width_text_box->align(FL_ALIGN_RIGHT);
				output_width_text_box->color(FL_DARK2);
			}

			Fl_Int_Input *output_width_input = new Fl_Int_Input(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, height_text_box->y() + height_text_box->h() + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "output_width_input");
			{
				output_width_input->labeltype(FL_NO_LABEL);
				output_width_input->value("3840");
			}

			Fl_Box *output_height_text_box = new Fl_Box(FL_FLAT_BOX, capture_config_x, output_width_text_box->y() + output_width_text_box->h() + new_configuration_margin_h, new_configuration_margin_w, new_configuration_btn_h, "Output_Height");
			{
				output_height_text_box->align(FL_ALIGN_RIGHT);
				output_height_text_box->color(FL_DARK2);
			}

			Fl_Int_Input *output_height_input = new Fl_Int_Input(new_configuration_btn_w + new_configuration_margin_w + new_configuration_margin_w, output_width_text_box->y() + output_width_text_box->h() + new_configuration_margin_h, new_configuration_btn_w, new_configuration_btn_h, "output_height_input");
			{
				output_height_input->labeltype(FL_NO_LABEL);
				output_height_input->value("1920");
			}

			Fl_Button *save_btn = new Fl_Button(new_configuration_w - new_configuration_btn_w - new_configuration_margin_w, output_height_input->y() + output_height_input->h() + new_configuration_long_margin_h, new_configuration_btn_w, new_configuration_btn_h, "Save");
			save_btn->callback(new_configuratio_btn_callback, win);

			new_configuration->end();

			new_configuration->labeltype(FL_NO_LABEL);
		}
	}
	win->new_configuration_win->end();
}

void initColorSettingLayout(Quram_Fl_Window* win) {
	int color_setting_window_x = win->x() + (win->w() / 2) - (color_setting_window_w / 2);
	int color_setting_window_y = win->y() + (win->h() / 2) - (color_setting_window_h / 2);

	win->color_setting_win = new Fl_Window(color_setting_window_x, color_setting_window_y, color_setting_window_w, color_setting_window_h, "Color Setting");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, color_setting_window_w, color_setting_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *color_setting = new Fl_Group(color_setting_x, color_setting_y, color_setting_w, color_setting_h, "color_setting");
		{
			Fl_Button* btn_sources = new Fl_Button(color_setting_x + color_setting_margin_w, color_setting_y + color_setting_margin_h, color_setting_btn_w, color_setting_btn_h, "Sources");
			{
				btn_sources->callback(mainmenu_btn_callback, win);
			}

			Fl_Button* btn_panorama = new Fl_Button(color_setting_x + color_setting_margin_w, btn_sources->y() + color_setting_btn_h + color_setting_margin_h, color_setting_btn_w, color_setting_btn_h, "Panorama");
			{
				btn_panorama->callback(mainmenu_btn_callback, win);
			}

			Fl_Button* btn_interactive = new Fl_Button(color_setting_x + color_setting_margin_w, btn_panorama->y() + color_setting_btn_h + color_setting_margin_h, color_setting_btn_w, color_setting_btn_h, "Interactive");
			{
				btn_interactive->callback(mainmenu_btn_callback, win);
			}

			Fl_Group *round_button = new Fl_Group(btn_sources->x() + color_setting_btn_w + color_setting_long_margin_w, color_setting_y + color_setting_margin_h, round_button_w, round_button_h, "round_button");
			{
				Fl_Round_Button* btn_input1 = new Fl_Round_Button(round_button->x() + color_setting_margin_w, round_button->y() + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "1");
				{
					btn_input1->type(102);
					btn_input1->labelsize(16);
					btn_input1->down_box(FL_ROUND_DOWN_BOX);
					btn_input1->setonly();
					btn_input1->callback(color_setting_btn_callback, win);
				}

				Fl_Round_Button* btn_input2 = new Fl_Round_Button(btn_input1->x() + color_setting_round_btn_w + color_setting_margin_w, round_button->y() + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "2");
				{
					btn_input2->type(102);
					btn_input2->labelsize(16);
					btn_input2->down_box(FL_ROUND_DOWN_BOX);
					btn_input2->callback(color_setting_btn_callback, win);
				}

				Fl_Round_Button* btn_input3 = new Fl_Round_Button(btn_input2->x() + color_setting_round_btn_w + color_setting_margin_w, round_button->y() + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "3");
				{
					btn_input3->type(102);
					btn_input3->labelsize(16);
					btn_input3->down_box(FL_ROUND_DOWN_BOX);
					btn_input3->callback(color_setting_btn_callback, win);
				}

				Fl_Round_Button* btn_input4 = new Fl_Round_Button(round_button->x() + color_setting_margin_w, btn_input1->y() + color_setting_round_btn_h + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "4");
				{
					btn_input4->type(102);
					btn_input4->labelsize(16);
					btn_input4->down_box(FL_ROUND_DOWN_BOX);
					btn_input4->callback(color_setting_btn_callback, win);
				}

				Fl_Round_Button* btn_input5 = new Fl_Round_Button(btn_input4->x() + color_setting_round_btn_w + color_setting_margin_w, btn_input1->y() + color_setting_round_btn_h + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "5");
				{
					btn_input5->type(102);
					btn_input5->labelsize(16);
					btn_input5->down_box(FL_ROUND_DOWN_BOX);
					btn_input5->callback(color_setting_btn_callback, win);
				}

				Fl_Round_Button* btn_input6 = new Fl_Round_Button(btn_input5->x() + color_setting_round_btn_w + color_setting_margin_w, btn_input1->y() + color_setting_round_btn_h + color_setting_margin_h, color_setting_round_btn_w, color_setting_round_btn_h, "6");
				{
					btn_input6->type(102);
					btn_input6->labelsize(16);
					btn_input6->down_box(FL_ROUND_DOWN_BOX);
					btn_input6->callback(color_setting_btn_callback, win);
				}

				round_button->end();

				round_button->labeltype(FL_NO_LABEL);
			}

			Fl_Value_Slider* slider_red = new Fl_Value_Slider(round_button->x(), round_button->y() + round_button->h() + color_setting_margin_h, color_setting_slider_w, color_setting_slider_h, "RED");
			{
				slider_red->type(FL_VERT_NICE_SLIDER);
				slider_red->textsize(16);
				slider_red->bounds(20, -20);
				slider_red->step(1);
				slider_red->value(0);
				slider_red->callback(color_setting_slider_callback, win);
			}

			Fl_Value_Slider* slider_green = new Fl_Value_Slider(slider_red->x() + color_setting_slider_w + color_setting_margin_w, round_button->y() + round_button->h() + color_setting_margin_h, color_setting_slider_w, color_setting_slider_h, "GREEN");
			{
				slider_green->type(FL_VERT_NICE_SLIDER);
				slider_green->textsize(16);
				slider_green->bounds(20, -20);
				slider_green->step(1);
				slider_green->value(0);
				slider_green->callback(color_setting_slider_callback, win);
			}

			Fl_Value_Slider* slider_blue = new Fl_Value_Slider(slider_green->x() + color_setting_slider_w + color_setting_margin_w, round_button->y() + round_button->h() + color_setting_margin_h, color_setting_slider_w, color_setting_slider_h, "BLUE");
			{
				slider_blue->type(FL_VERT_NICE_SLIDER);
				slider_blue->textsize(16);
				slider_blue->bounds(20, -20);
				slider_blue->step(1);
				slider_blue->value(0);
				slider_blue->callback(color_setting_slider_callback, win);
			}

			color_setting->end();

			color_setting->labeltype(FL_NO_LABEL);
		}
	}
	win->color_setting_win->end();
}

void initGainCurveManualLayout(Quram_Fl_Window* win) {
	int gain_curve_manual_window_x = win->x() + (win->w() / 2) - (gain_curve_manual_window_w / 2);
	int gain_curve_manual_window_y = win->y() + (win->h() / 2) - (gain_curve_manual_window_h / 2);

	win->gain_curve_manual_win = new Fl_Window(gain_curve_manual_window_x, gain_curve_manual_window_y, gain_curve_manual_window_w, gain_curve_manual_window_h, "Curve Setting");
	{
		Fl_Box *backgournd = new Fl_Box(FL_FLAT_BOX, 0, 0, gain_curve_manual_window_w, gain_curve_manual_window_h, "");
		{
			backgournd->color(FL_DARK2);
		}

		Fl_Group *gain_curve_manual = new Fl_Group(gain_curve_manual_x, gain_curve_manual_y, gain_curve_manual_w, gain_curve_manual_h, "gain_curve_manual");
		{
			unsigned char* buf = (unsigned char*)malloc(gain_curve_w * gain_curve_h * 3);
			memset(buf, 0x00, gain_curve_w * gain_curve_h * 3);
			Fl_RGB_Image *capture_image = new Fl_RGB_Image(buf, gain_curve_w, gain_curve_h);

			Fl_Group *gain_curve = new Fl_Group(gain_curve_manual_x, gain_curve_manual_y, gain_curve_w + gain_curve_manual_margin_w, gain_curve_h + gain_curve_manual_margin_h + gain_curve_manual_margin_h, "gain_curve");
			{
				Fl_Box *gain_curve_graph = new Fl_Box(gain_curve_manual_x + gain_curve_manual_margin_w, gain_curve_manual_y + gain_curve_manual_margin_h + gain_curve_manual_margin_h, gain_curve_w, gain_curve_h, "Gain Curve Graph");
				{
					gain_curve_graph->image(capture_image);
					gain_curve_graph->align(FL_ALIGN_INSIDE);
				}

				gain_curve->end();

				gain_curve->labeltype(FL_NO_LABEL);
			}

			Fl_Group *gain_curve_info = new Fl_Group(gain_curve->x() + gain_curve->w() + gain_curve_manual_margin_w, gain_curve->y(), gain_curve_points_w, gain_curve_points_h, "gain_curve_info");
			{

				Fl_Box *gain_channel_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_info->x(), gain_curve_info->y(), gain_curve_manual_text_w, gain_curve_manual_text_h, "Input : ");
				{
					gain_channel_text_box->align(FL_ALIGN_RIGHT);
					gain_channel_text_box->color(FL_DARK2);
				}

				Fl_Box *gain_rgb_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_info->x(), gain_channel_text_box->y() + gain_curve_manual_text_h, gain_curve_manual_text_w, gain_curve_manual_text_h, "Color : ");
				{
					gain_rgb_text_box->align(FL_ALIGN_RIGHT);
					gain_rgb_text_box->color(FL_DARK2);
				}

				Fl_Box *gain_point_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_info->x(), gain_rgb_text_box->y() + gain_curve_manual_text_h, gain_curve_manual_text_w, gain_curve_manual_text_h, "Point : ");
				{
					gain_point_text_box->align(FL_ALIGN_RIGHT);
					gain_point_text_box->color(FL_DARK2);
				}

				gain_curve_info->end();

				gain_curve_info->labeltype(FL_NO_LABEL);
			}

			Fl_Group *gain_curve_points = new Fl_Group(gain_curve->x() + gain_curve->w() + gain_curve_manual_margin_w, gain_curve->y() + gain_curve->h() - gain_curve_points_h, gain_curve_points_w, gain_curve_points_h, "gain_curve_points");
			{
				Fl_Group *point_round_button = new Fl_Group(gain_curve_points->x(), gain_curve_points->y(), point_round_button_w, point_round_button_h, "point_round_button");
				{
					Fl_Round_Button* btn_input_p1 = new Fl_Round_Button(gain_curve_points->x() + gain_curve_manual_margin_w, gain_curve_points->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "P1");
					{
						btn_input_p1->type(102);
						btn_input_p1->down_box(FL_ROUND_DOWN_BOX);
						btn_input_p1->callback(gain_curve_manual_btn_callback, win);
					}

					Fl_Round_Button* btn_input_p2 = new Fl_Round_Button(gain_curve_points->x() + gain_curve_manual_margin_w, btn_input_p1->y() + btn_input_p1->h(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "P2");
					{
						btn_input_p2->type(102);
						btn_input_p2->down_box(FL_ROUND_DOWN_BOX);
						btn_input_p2->callback(gain_curve_manual_btn_callback, win);
					}

					Fl_Round_Button* btn_input_p3 = new Fl_Round_Button(gain_curve_points->x() + gain_curve_manual_margin_w, btn_input_p2->y() + btn_input_p2->h(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "P3");
					{
						btn_input_p3->type(102);
						btn_input_p3->down_box(FL_ROUND_DOWN_BOX);
						btn_input_p3->callback(gain_curve_manual_btn_callback, win);
					}

					point_round_button->end();

					point_round_button->labeltype(FL_NO_LABEL);
				}

				Fl_Group *gain_curve_point1 = new Fl_Group(gain_curve_points->x() + gain_curve_manual_long_margin_w, gain_curve_points->y(), gain_curve_point_w, gain_curve_point_h, "gain_curve_point1");
				{
					Fl_Box *left_bracket_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_point1->x() + gain_curve_manual_text_w, gain_curve_point1->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, "(");
					{
						left_bracket_text_box->align(FL_ALIGN_LEFT);
						left_bracket_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_x = new Fl_Float_Input(left_bracket_text_box->x(), gain_curve_point1->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_x");
					{
						point_x->labeltype(FL_NO_LABEL);
					}

					Fl_Box *comma_text_box = new Fl_Box(FL_FLAT_BOX, point_x->x() + point_x->w(), gain_curve_point1->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ",");
					{
						comma_text_box->align(FL_ALIGN_CENTER);
						comma_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_y = new Fl_Float_Input(comma_text_box->x() + gain_curve_manual_text_w, gain_curve_point1->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_y");
					{
						point_y->labeltype(FL_NO_LABEL);
					}

					Fl_Box *right_bracket_text_box = new Fl_Box(FL_FLAT_BOX, point_y->x() + point_y->w(), gain_curve_point1->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ")");
					{
						right_bracket_text_box->align(FL_ALIGN_CENTER);
						right_bracket_text_box->color(FL_DARK2);
					}

					gain_curve_point1->end();

					gain_curve_point1->labeltype(FL_NO_LABEL);
				}

				Fl_Group *gain_curve_point2 = new Fl_Group(gain_curve_points->x() + gain_curve_manual_long_margin_w, gain_curve_point1->y() + gain_curve_point1->h(), gain_curve_point_w, gain_curve_point_h, "gain_curve_point2");
				{
					Fl_Box *left_bracket_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_point2->x() + gain_curve_manual_text_w, gain_curve_point2->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, "(");
					{
						left_bracket_text_box->align(FL_ALIGN_LEFT);
						left_bracket_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_x = new Fl_Float_Input(left_bracket_text_box->x(), gain_curve_point2->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_x");
					{
						point_x->labeltype(FL_NO_LABEL);
					}

					Fl_Box *comma_text_box = new Fl_Box(FL_FLAT_BOX, point_x->x() + point_x->w(), gain_curve_point2->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ",");
					{
						comma_text_box->align(FL_ALIGN_CENTER);
						comma_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_y = new Fl_Float_Input(comma_text_box->x() + gain_curve_manual_text_w, gain_curve_point2->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_y");
					{
						point_y->labeltype(FL_NO_LABEL);
					}

					Fl_Box *right_bracket_text_box = new Fl_Box(FL_FLAT_BOX, point_y->x() + point_y->w(), gain_curve_point2->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ")");
					{
						right_bracket_text_box->align(FL_ALIGN_CENTER);
						right_bracket_text_box->color(FL_DARK2);
					}

					gain_curve_point2->end();

					gain_curve_point2->labeltype(FL_NO_LABEL);
				}

				Fl_Group *gain_curve_point3 = new Fl_Group(gain_curve_points->x() + gain_curve_manual_long_margin_w, gain_curve_point2->y() + gain_curve_point2->h(), gain_curve_point_w, gain_curve_point_h, "gain_curve_point3");
				{
					Fl_Box *left_bracket_text_box = new Fl_Box(FL_FLAT_BOX, gain_curve_point3->x() + gain_curve_manual_text_w, gain_curve_point3->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, "(");
					{
						left_bracket_text_box->align(FL_ALIGN_LEFT);
						left_bracket_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_x = new Fl_Float_Input(left_bracket_text_box->x(), gain_curve_point3->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_x");
					{
						point_x->labeltype(FL_NO_LABEL);
					}

					Fl_Box *comma_text_box = new Fl_Box(FL_FLAT_BOX, point_x->x() + point_x->w(), gain_curve_point3->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ",");
					{
						comma_text_box->align(FL_ALIGN_CENTER);
						comma_text_box->color(FL_DARK2);
					}

					Fl_Float_Input *point_y = new Fl_Float_Input(comma_text_box->x() + gain_curve_manual_text_w, gain_curve_point3->y(), gain_curve_manual_input_w, gain_curve_manual_input_h, "point_y");
					{
						point_y->labeltype(FL_NO_LABEL);
					}

					Fl_Box *right_bracket_text_box = new Fl_Box(FL_FLAT_BOX, point_y->x() + point_y->w(), gain_curve_point3->y(), gain_curve_manual_margin_w, gain_curve_manual_text_h, ")");
					{
						right_bracket_text_box->align(FL_ALIGN_CENTER);
						right_bracket_text_box->color(FL_DARK2);
					}

					gain_curve_point3->end();

					gain_curve_point3->labeltype(FL_NO_LABEL);
				}

				gain_curve_points->end();

				gain_curve_points->labeltype(FL_NO_LABEL);
			}

			Fl_Group *channel_round_button = new Fl_Group(gain_curve_manual_x + gain_curve_manual_margin_w, gain_curve->y() + gain_curve->h() + gain_curve_manual_margin_h, channel_round_button_w, channel_round_button_h, "channel_round_button");
			{
				Fl_Round_Button* btn_input1 = new Fl_Round_Button(channel_round_button->x(), channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "1");
				{
					btn_input1->type(102);
					btn_input1->labelsize(16);
					btn_input1->down_box(FL_ROUND_DOWN_BOX);
					btn_input1->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input2 = new Fl_Round_Button(btn_input1->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "2");
				{
					btn_input2->type(102);
					btn_input2->labelsize(16);
					btn_input2->down_box(FL_ROUND_DOWN_BOX);
					btn_input2->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input3 = new Fl_Round_Button(btn_input2->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "3");
				{
					btn_input3->type(102);
					btn_input3->labelsize(16);
					btn_input3->down_box(FL_ROUND_DOWN_BOX);
					btn_input3->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input4 = new Fl_Round_Button(btn_input3->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "4");
				{
					btn_input4->type(102);
					btn_input4->labelsize(16);
					btn_input4->down_box(FL_ROUND_DOWN_BOX);
					btn_input4->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input5 = new Fl_Round_Button(btn_input4->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "5");
				{
					btn_input5->type(102);
					btn_input5->labelsize(16);
					btn_input5->down_box(FL_ROUND_DOWN_BOX);
					btn_input5->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input6 = new Fl_Round_Button(btn_input5->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, channel_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "6");
				{
					btn_input6->type(102);
					btn_input6->labelsize(16);
					btn_input6->down_box(FL_ROUND_DOWN_BOX);
					btn_input6->callback(gain_curve_manual_btn_callback, win);
				}

				channel_round_button->end();

				channel_round_button->labeltype(FL_NO_LABEL);
			}

			Fl_Group *rgb_round_button = new Fl_Group(gain_curve_manual_x + gain_curve_manual_margin_w, channel_round_button->y() + channel_round_button->h(), rgb_round_button_w, rgb_round_button_h, "rgb_round_button");
			{
				Fl_Round_Button* btn_input_r = new Fl_Round_Button(rgb_round_button->x(), rgb_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "R");
				{
					btn_input_r->type(102);
					btn_input_r->labelsize(16);
					btn_input_r->down_box(FL_ROUND_DOWN_BOX);
					btn_input_r->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input_g = new Fl_Round_Button(btn_input_r->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, rgb_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "G");
				{
					btn_input_g->type(102);
					btn_input_g->labelsize(16);
					btn_input_g->down_box(FL_ROUND_DOWN_BOX);
					btn_input_g->callback(gain_curve_manual_btn_callback, win);
				}

				Fl_Round_Button* btn_input_b = new Fl_Round_Button(btn_input_g->x() + gain_curve_manual_round_btn_w + gain_curve_manual_margin_w, rgb_round_button->y(), gain_curve_manual_round_btn_w, gain_curve_manual_round_btn_h, "B");
				{
					btn_input_b->type(102);
					btn_input_b->labelsize(16);
					btn_input_b->down_box(FL_ROUND_DOWN_BOX);
					btn_input_b->callback(gain_curve_manual_btn_callback, win);
				}

				rgb_round_button->end();

				rgb_round_button->labeltype(FL_NO_LABEL);
			}

			Fl_Button *btn_apply = new Fl_Button(gain_curve_manual_w - gain_curve_manual_btn_w - gain_curve_manual_margin_w, gain_curve_manual_h - gain_curve_manual_btn_h - gain_curve_manual_margin_h, gain_curve_manual_btn_w, gain_curve_manual_btn_h, "Apply");
			btn_apply->callback(gain_curve_manual_btn_callback, win);

			gain_curve_manual->end();

			gain_curve_manual->labeltype(FL_NO_LABEL);
		}
	}

	win->gain_curve_manual_win->end();
}

bool initQVREngine()
{
	return QVS_Engine::QVS_InitEngineManager();
}
void setCallbacks()
{
	QVS_Engine::QVS_setCallback_snapshot(onCallbackDone);
	QVS_Engine::QVS_setCallback_doManualGainMode(onCallbackDone);
	//QVS_Engine::QVS_setCallback_quramCalibMode(onCallbackDone);
	QVS_Engine::QVS_setCallback_gainMode(onCallbackDone);
	QVS_Engine::QVS_setCallback_seamMode(onCallbackDone);
	QVS_Engine::QVS_setCallback_autoGainMode(onCallbackDone);
	QVS_Engine::QVS_setCallback_editOutput(onCallbackDone);
	QVS_Engine::QVS_setCallback_calibration(onCallbackDone);
	QVS_Engine::QVS_setCallback_history(onCallbackDone);
	QVS_Engine::QVS_setCallback_load(onCallbackDone);

	/*QVS_Engine::QVS_setCallback_startOutput(onCallbackDone);
	QVS_Engine::QVS_setCallback_startEncoding(onCallbackDone);
	QVS_Engine::QVS_setCallback_startStreaming(onCallbackDone);
	QVS_Engine::QVS_setCallback_audioBufferOffset(onCallbackDone);
	QVS_Engine::QVS_setCallback_videoBufferOffset(onCallbackDone);*/
}
void onCallbackDone(int msg)
{
	printf("onCallbackDone: msg = %d\n", msg);
	switch (msg)
	{
	case QVS_MSG_SNAPSHOT_DONE:
		break;

	case QVS_MSG_MANUAL_GAIN_MODE_DONE:
		break;

	case QVS_MSG_QURAM_CALIB_MODE_DONE:
		break;

	case QVS_MSG_GAIN_DONE:
		break;

	case QVS_MSG_SEAM_DONE:
		break;

	case QVS_MSG_AUTO_GAIN_DONE:
		break;

	case QVS_MSG_CALIBRATION_DONE:
		break;
	case QVS_MSG_CALIBRATION_FOV_FAIL:
	case QVS_MSG_CALIBRATION_FOV_WARNING:
		fl_message((char*)"Calibration FOV WARNING!");
		break;
	case QVS_MSG_CALIBRATION_FAIL:
		fl_message((char*)"Calibration fail!");
		break;
	case QVS_MSG_DO_LOAD:
		break;
	case QVS_MSG_DONE_LOAD:
		break;

	/*case QVS_MSG_OUTPUT_DONE:
		break;

	case QVS_MSG_ENCODING_DONE:
		break;

	case QVS_MSG_STREAMING_DONE:
		break;

	case QVS_MSG_AUDIO_BUF_OFFSET_DONE:
		break;

	case QVS_MSG_VIDEO_BUF_OFFSET_DONE:
		break;*/
	}
}
