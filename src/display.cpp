#include "main.h"
#include "display.hpp"
#include "autons.hpp"

#define DO_NOT_RUN 1982

void placeHolder(void){};

int displayMode = 0;
int selectedProgram = DO_NOT_RUN;
int autoType = 0;
bool isLeft = false;
bool selected = false;
bool driverSkillsMode = false;
const char* leftTitles[] = {"Five Discs, One Roller", "Disabled"};
const char* rightTitles[] = {"Two Discs One Roller", "Disabled"};
const char* skillsTitles[] = {"Programming Skills","Driver Skills"};

void (*leftScripts[])() = {leftAuton, placeHolder};
void (*rightScripts[])() = {rightAuton, placeHolder};
void (*skillsScripts[])() = {pSkills,dSkills};

lv_obj_t* selectAutoScreen = lv_obj_create(NULL, NULL);
lv_obj_t* mainMenuScreen = lv_obj_create(NULL, NULL);
lv_obj_t* debugMenu = lv_obj_create(NULL, NULL);

lv_obj_t* leftOption1;
lv_obj_t* leftOption2;

lv_obj_t* rightOption1;
lv_obj_t* rightOption2;

lv_obj_t* skillsOption1;
lv_obj_t* skillsOption2;


lv_obj_t* catTempMeter;
lv_obj_t* catTempLabel;
lv_obj_t* intakeTempMeter;
lv_obj_t* intakeTempLabel;
lv_obj_t* driveTempMeter;
lv_obj_t* driveTempLabel;

void uncheckColor(bool left, bool right, bool skills) {
	if (left) {
		lv_cb_set_checked(leftOption1, false);
		lv_cb_set_checked(leftOption2, false);
	}
	if (right) {
		lv_cb_set_checked(rightOption1, false);
		lv_cb_set_checked(rightOption2, false);
	}
	if (skills) {
		lv_cb_set_checked(skillsOption1, false);
		lv_cb_set_checked(skillsOption2, false);
	}
}

lv_action_t cancel() {
	// Run Code To Exit Selection Menu And Set "selectedProgram" to "DO_NOT_RUN"
	selectedProgram = DO_NOT_RUN;
	lv_scr_load(mainMenuScreen);
	return (lv_action_t)LV_RES_OK;
}

lv_action_t selectAuto() {
	// Run Code To Exit Selection Menu. "selectedProgram" Is Already Set
	if (selectedProgram == DO_NOT_RUN) {
		lv_obj_t* mbox1 = lv_mbox_create(selectAutoScreen, NULL);
		
		lv_mbox_set_text(mbox1,
		                 "Don't Be An Idiot\n"
		                 "Select An Autonomous\n"
		                 "Or Tap 'Cancel'\n");
		static const char* btns[] = {"\221Close", ""};
		lv_mbox_add_btns(mbox1, btns, NULL);
		lv_obj_set_width(mbox1, 250);
		lv_obj_align(mbox1, NULL, LV_ALIGN_IN_TOP_MID, 10, 10);
	} else {
		lv_scr_load(mainMenuScreen);
	}
	return (lv_action_t)LV_RES_OK;
}


lv_action_t setRed(lv_obj_t* checkBox) {
	if (lv_cb_is_checked(leftOption1) && leftOption1 == checkBox) {
		lv_cb_set_checked(leftOption2, false);
		uncheckColor(false, true, true);
		driverSkillsMode = false;
		selectedProgram = 0;
	} else if (lv_cb_is_checked(leftOption2) && leftOption2 == checkBox) {
		lv_cb_set_checked(leftOption1, false);
		uncheckColor(false, true, true);
		driverSkillsMode = false;
		selectedProgram = 1;
	} 
	autoType = AUTONOMOUS_LEFT;
	selected = true;
	return (lv_action_t)LV_RES_OK;
}

lv_action_t setBlue(lv_obj_t* checkBox) {
	if (lv_cb_is_checked(rightOption1) && rightOption1 == checkBox) {
		lv_cb_set_checked(rightOption2, false);
		uncheckColor(true, false, true);
		driverSkillsMode = false;
		selectedProgram = 0;
	} else if (lv_cb_is_checked(rightOption2) && rightOption2 == checkBox) {
		lv_cb_set_checked(rightOption1, false);
		uncheckColor(true, false, true);
		driverSkillsMode = false;
		selectedProgram = 1;
	} 
	autoType = AUTONOMOUS_RIGHT;
	selected = true;
	return (lv_action_t)LV_RES_OK;
}

lv_action_t setSkills(lv_obj_t* checkBox) {
	if (lv_cb_is_checked(skillsOption1) && skillsOption1 == checkBox) {
		lv_cb_set_checked(skillsOption2, false);
		uncheckColor(true, true, false);
		driverSkillsMode = false;
		selectedProgram = 0;
	} else if (lv_cb_is_checked(skillsOption2) && skillsOption2 == checkBox) {
		lv_cb_set_checked(skillsOption1, false);
		uncheckColor(true, true, false);
		driverSkillsMode = false;
		selectedProgram = 1;
	} 
	autoType = AUTONOMOUS_SKILLS;
	selected = true;
	return (lv_action_t)LV_RES_OK;
}

static lv_res_t goToAutoSelect(lv_obj_t* btn) {
	lv_scr_load(selectAutoScreen);
	return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_res_t goToDebug(lv_obj_t* btn) {
	lv_scr_load(debugMenu);
	return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_res_t goToMainMenu(lv_obj_t* btn) {
	lv_scr_load(mainMenuScreen);
	return LV_RES_OK; /*Return OK if the button is not deleted*/
}

void displayInit(void) {
	selectAutoScreen = lv_scr_act();
	lv_scr_load(selectAutoScreen);
	lv_theme_t* th = lv_theme_alien_init(210, &lv_font_dejavu_20);  // Set a HUE value and a Font for the Night Theme
	lv_theme_set_current(th);
	lv_obj_t* tabview;
	tabview = lv_tabview_create(selectAutoScreen, NULL);

	// Add 3 tabs (the tabs are page (lv_page) and can be scrolled
	lv_obj_t* redTab = lv_tabview_add_tab(tabview, "Left");
	lv_obj_t* blueTab = lv_tabview_add_tab(tabview, "Right");
	lv_obj_t* skillsTab = lv_tabview_add_tab(tabview, "Skills");

	static lv_style_t no_border;
	lv_style_copy(&no_border, &lv_style_pretty_color);
	no_border.glass = 1;
	no_border.body.empty = 1;
	no_border.body.padding.inner = -15;
	no_border.body.border.width = 0;
	static lv_style_t style_border;
	lv_style_copy(&style_border, &lv_style_pretty_color);
	style_border.glass = 1;
	style_border.body.empty = 1;
	style_border.body.padding.inner = 5;

	lv_obj_t* redContent;
	lv_obj_t* blueContent;
	lv_obj_t* skillsContent;
	lv_obj_t* redSideContent;
	lv_obj_t* blueSideContent;
	lv_obj_t* skillsSideContent;
	lv_obj_t* redSelectButtonContent;
	lv_obj_t* blueSelectButtonContent;
	lv_obj_t* skillsSelectButtonContent;
	lv_obj_t* redCancelButtonContent;
	lv_obj_t* blueCancelButtonContent;
	lv_obj_t* skillsCancelButtonContent;

	redContent = lv_cont_create(redTab, NULL);
	blueContent = lv_cont_create(blueTab, NULL);
	skillsContent = lv_cont_create(skillsTab, NULL);
	redSideContent = lv_cont_create(redTab, NULL);
	blueSideContent = lv_cont_create(blueTab, NULL);

	redSelectButtonContent = lv_cont_create(redTab, NULL);
	blueSelectButtonContent = lv_cont_create(blueTab, NULL);
	skillsSelectButtonContent = lv_cont_create(skillsTab, NULL);

	redCancelButtonContent = lv_cont_create(redTab, NULL);
	blueCancelButtonContent = lv_cont_create(blueTab, NULL);
	skillsCancelButtonContent = lv_cont_create(skillsTab, NULL);

	lv_cont_set_layout(redContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(redContent, true, true);
	lv_obj_set_style(redContent, &style_border);

	lv_cont_set_layout(blueContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(blueContent, true, true);
	lv_obj_set_style(blueContent, &style_border);

	lv_cont_set_layout(skillsContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(skillsContent, true, true);
	lv_obj_set_style(skillsContent, &style_border);

	lv_cont_set_layout(redSideContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(redSideContent, true, true);
	lv_obj_set_style(redSideContent, &style_border);

	lv_cont_set_layout(blueSideContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(blueSideContent, true, true);
	lv_obj_set_style(blueSideContent, &style_border);

	lv_cont_set_layout(redSelectButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(redSelectButtonContent, false, false);
	lv_obj_set_style(redSelectButtonContent, &no_border);

	lv_cont_set_layout(redCancelButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(redCancelButtonContent, true, true);
	lv_obj_set_style(redCancelButtonContent, &no_border);

	lv_cont_set_layout(blueSelectButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(blueSelectButtonContent, false, false);
	lv_obj_set_style(blueSelectButtonContent, &no_border);

	lv_cont_set_layout(blueCancelButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(blueCancelButtonContent, true, true);
	lv_obj_set_style(blueCancelButtonContent, &no_border);

	lv_cont_set_layout(skillsSelectButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(skillsSelectButtonContent, false, false);
	lv_obj_set_style(skillsSelectButtonContent, &no_border);

	lv_cont_set_layout(skillsCancelButtonContent, LV_LAYOUT_COL_L);
	lv_cont_set_fit(skillsCancelButtonContent, true, true);
	lv_obj_set_style(skillsCancelButtonContent, &no_border);

	/**************************
	 * Create check boxes
	 *************************/

	// Create check box

	
	if (sizeof(leftTitles) / sizeof(leftTitles[0]) > 0) {
		leftOption1 = lv_cb_create(redContent, NULL);
		lv_cb_set_text(leftOption1, leftTitles[0]);
		lv_cb_set_action(leftOption1, (lv_action_t)setRed);
	}

	if (sizeof(leftTitles) / sizeof(leftTitles[0]) > 1) {
		leftOption2 = lv_cb_create(redContent, NULL);
		lv_cb_set_text(leftOption2, leftTitles[1]);
		lv_cb_set_action(leftOption2, (lv_action_t)setRed);
	}

	

	if (sizeof(rightTitles) / sizeof(rightTitles[0]) > 0) {
		rightOption1 = lv_cb_create(blueContent, NULL);
		lv_cb_set_text(rightOption1, rightTitles[0]);
		lv_cb_set_action(rightOption1, (lv_action_t)setBlue);
	}

	if (sizeof(rightTitles) / sizeof(rightTitles[0]) > 1) {
		rightOption2 = lv_cb_create(blueContent, NULL);
		lv_cb_set_text(rightOption2, rightTitles[1]);
		lv_cb_set_action(rightOption2, (lv_action_t)setBlue);
	}


	if (sizeof(skillsTitles) / sizeof(skillsTitles[0]) > 0) {
		skillsOption1 = lv_cb_create(skillsContent, NULL);
		lv_cb_set_text(skillsOption1, skillsTitles[0]);
		lv_cb_set_action(skillsOption1, (lv_action_t)setSkills);
	}

	if (sizeof(skillsTitles) / sizeof(skillsTitles[0]) > 1) {
		skillsOption2 = lv_cb_create(skillsContent, NULL);
		lv_cb_set_text(skillsOption2, skillsTitles[1]);
		lv_cb_set_action(skillsOption2, (lv_action_t)setSkills);
	}


	// Create Select and Cancel button
	// lv_obj_t * label = lv_label_create(selectAutoScreen, NULL);
	lv_obj_t* label;
	lv_obj_t* selectBtn = lv_btn_create(redSelectButtonContent, NULL);
	lv_obj_align(selectBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(selectBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(selectBtn, LV_BTN_ACTION_CLICK, (lv_action_t)selectAuto);

	lv_obj_t* blueSelectBtn = lv_btn_create(blueSelectButtonContent, NULL);
	lv_obj_align(blueSelectBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(blueSelectBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(blueSelectBtn, LV_BTN_ACTION_CLICK, (lv_action_t)selectAuto);

	lv_obj_t* skillsSelectBtn = lv_btn_create(skillsSelectButtonContent, NULL);
	lv_obj_align(skillsSelectBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(skillsSelectBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(skillsSelectBtn, LV_BTN_ACTION_CLICK, (lv_action_t)selectAuto);

	// Add a label to the button
	label = lv_label_create(selectBtn, NULL);
	lv_label_set_text(label, "Select");
	label = lv_label_create(blueSelectBtn, NULL);
	lv_label_set_text(label, "Select");
	label = lv_label_create(skillsSelectBtn, NULL);
	lv_label_set_text(label, "Select");

	lv_obj_t* cancelBtn = lv_btn_create(redCancelButtonContent, NULL);
	lv_obj_align(cancelBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(cancelBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(cancelBtn, LV_BTN_ACTION_CLICK, (lv_action_t)cancel);

	lv_obj_t* blueCancelBtn = lv_btn_create(blueCancelButtonContent, NULL);
	lv_obj_align(blueCancelBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(blueCancelBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(blueCancelBtn, LV_BTN_ACTION_CLICK, (lv_action_t)cancel);

	lv_obj_t* skillsCancelBtn = lv_btn_create(skillsCancelButtonContent, NULL);
	lv_obj_align(skillsCancelBtn, label, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_free_num(skillsCancelBtn, 1); /*Set a unique number for the button*/
	lv_btn_set_action(skillsCancelBtn, LV_BTN_ACTION_CLICK, (lv_action_t)cancel);

	// Add a label to the button
	label = lv_label_create(cancelBtn, NULL);
	lv_label_set_text(label, "Cancel");
	label = lv_label_create(blueCancelBtn, NULL);
	lv_label_set_text(label, "Cancel");
	label = lv_label_create(skillsCancelBtn, NULL);
	lv_label_set_text(label, "Cancel");

	// Set The Size Of The Select And Cancel Buttons
	lv_obj_set_size(selectBtn, 100, 50);
	lv_obj_set_size(blueSelectBtn, 100, 50);
	lv_obj_set_size(skillsSelectBtn, 100, 50);

	lv_obj_set_size(cancelBtn, 100, 50);
	lv_obj_set_size(blueCancelBtn, 100, 50);
	lv_obj_set_size(skillsCancelBtn, 100, 50);

	lv_obj_align(redContent, NULL, LV_ALIGN_IN_LEFT_MID, 0, -50);
	lv_obj_align(blueContent, NULL, LV_ALIGN_IN_LEFT_MID, 0, -50);
	lv_obj_align(skillsContent, NULL, LV_ALIGN_IN_LEFT_MID, 0, -50);
	lv_obj_align(redSideContent, NULL, LV_ALIGN_IN_RIGHT_MID, 0, -25);
	lv_obj_align(blueSideContent, NULL, LV_ALIGN_IN_RIGHT_MID, 0, -25);
	lv_obj_align(redSelectButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 30);
	lv_obj_align(blueSelectButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 30);
	lv_obj_align(skillsSelectButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 30);

	lv_obj_align(redCancelButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -125, -25);
	lv_obj_align(blueCancelButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -125, -25);
	lv_obj_align(skillsCancelButtonContent, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -125, -25);

	// MAIN MENU BEGIN
	mainMenuScreen = lv_obj_create(NULL, NULL);
	lv_obj_t* title = lv_label_create(mainMenuScreen, NULL);
	lv_label_set_text(title, "Main Menu");
	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
	lv_obj_t* btn1 = lv_btn_create(mainMenuScreen, NULL);
	lv_cont_set_fit(btn1, true, true);
	lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 40);
	lv_obj_set_free_num(btn1, 1);
	lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, goToAutoSelect);

	label = lv_label_create(btn1, NULL);
	lv_label_set_text(label, "Select Autonomous");
	lv_page_set_sb_mode(redTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(blueTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(skillsTab, LV_SB_MODE_OFF);

	lv_obj_t* debugMenuBtn = lv_btn_create(mainMenuScreen, NULL);
	lv_cont_set_fit(debugMenuBtn, true, true);
	lv_obj_align(debugMenuBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 40);
	
	#if LV_USE_SWITCH

	static void event_handler(lv_obj_t * obj, lv_event_t event)
	{
		if(event == LV_EVENT_VALUE_CHANGED) {
			printf("State: %s\n", lv_switch_get_state(obj) ? "On" : "Off");
		}
	}

	void lv_ex_switch_1(void)
	{
		/*Create a switch and apply the styles*/
		lv_obj_t *sw1 = lv_switch_create(lv_scr_act(), NULL);
		lv_obj_align(sw1, NULL, LV_ALIGN_CENTER, 0, -50);
		lv_obj_set_event_cb(sw1, event_handler);

		/*Copy the first switch and turn it ON*/
		lv_obj_t *sw2 = lv_switch_create(lv_scr_act(), sw1);
		lv_switch_on(sw2, LV_ANIM_ON);
		lv_obj_align(sw2, NULL, LV_ALIGN_CENTER, 0, 50);
	}

	#endif

	lv_obj_set_free_num(debugMenuBtn, 1);
	lv_btn_set_action(debugMenuBtn, LV_BTN_ACTION_CLICK, goToDebug);
	

	label = lv_label_create(debugMenuBtn, NULL);
	lv_label_set_text(label, "Debug Menu");
	lv_page_set_sb_mode(redTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(blueTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(skillsTab, LV_SB_MODE_OFF);


	lv_obj_t* mainMenuBtn = lv_btn_create(debugMenu, NULL);
	
	lv_cont_set_fit(mainMenuBtn, true, true);
	lv_obj_align(mainMenuBtn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, 0);
	lv_obj_set_free_num(mainMenuBtn, 1);
	lv_btn_set_action(mainMenuBtn, LV_BTN_ACTION_CLICK, goToMainMenu);

	label = lv_label_create(mainMenuBtn, NULL);
	lv_label_set_text(label, "Main Menu");
	lv_page_set_sb_mode(redTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(blueTab, LV_SB_MODE_OFF);
	lv_page_set_sb_mode(skillsTab, LV_SB_MODE_OFF);

	static lv_style_t style_lmeter1;
	lv_style_copy(&style_lmeter1, &lv_style_pretty_color);
	style_lmeter1.line.width = 2;
	style_lmeter1.line.color = LV_COLOR_SILVER;
	style_lmeter1.body.main_color = LV_COLOR_LIME;
	style_lmeter1.body.grad_color = LV_COLOR_ORANGE;

	static lv_style_t style_label_small;
	lv_style_copy(&style_label_small, &lv_style_pretty);
	style_label_small.text.color = LV_COLOR_WHITE;

	catTempMeter = lv_lmeter_create(mainMenuScreen, NULL);
	lv_lmeter_set_range(catTempMeter, 15, 65);
	lv_lmeter_set_value(catTempMeter, 0);
	lv_lmeter_set_style(catTempMeter, &style_lmeter1);
	lv_obj_set_size(catTempMeter, 80, 80);
	lv_obj_align(catTempMeter, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);

	catTempLabel = lv_label_create(catTempMeter, NULL);
	lv_label_set_text(catTempLabel, "45 C");
	lv_label_set_style(catTempLabel, &style_label_small);
	lv_obj_align(catTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);

	title = lv_label_create(catTempMeter, NULL);
	lv_label_set_text(title, "Catapult");
	lv_label_set_style(title, &style_label_small);
	lv_obj_align(title, NULL, LV_ALIGN_CENTER, 0, 30);

	intakeTempMeter = lv_lmeter_create(mainMenuScreen, catTempMeter);
	lv_lmeter_set_value(intakeTempMeter, 0);
	lv_obj_align(intakeTempMeter, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);

	intakeTempLabel = lv_label_create(intakeTempMeter, catTempLabel);
	lv_label_set_text(intakeTempLabel, "60%");
	lv_obj_align(intakeTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);

	title = lv_label_create(intakeTempMeter, NULL);
	lv_label_set_text(title, "Intake");
	lv_label_set_style(title, &style_label_small);
	lv_obj_align(title, NULL, LV_ALIGN_CENTER, 0, 30);

	driveTempMeter = lv_lmeter_create(mainMenuScreen, intakeTempMeter);
	lv_lmeter_set_value(driveTempMeter, 0);
	lv_obj_align(driveTempMeter, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);

	driveTempLabel = lv_label_create(driveTempMeter, driveTempLabel);
	lv_label_set_text(driveTempLabel, "60%");
	lv_obj_align(driveTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);

	title = lv_label_create(driveTempMeter, NULL);
	lv_label_set_text(title, "Drive");
	lv_label_set_style(title, &style_label_small);
	lv_obj_align(title, NULL, LV_ALIGN_CENTER, 0, 30);
}
/*
void setCatTemp(int temp) {
	char str[32];
	sprintf(str, "%d C", temp);
	lv_lmeter_set_value(catTempMeter, temp);
	lv_label_set_text(catTempLabel, str);
	lv_obj_align(catTempMeter, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
	lv_obj_align(catTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

void setIntakeTemp(int temp) {
	char str[32];
	sprintf(str, "%d C", temp);
	lv_lmeter_set_value(intakeTempMeter, temp);
	lv_label_set_text(intakeTempLabel, str);
	lv_obj_align(intakeTempMeter, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);
	lv_obj_align(intakeTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}

void setDriveTemp(int temp) {
	char str[32];
	sprintf(str, "%d C", temp);
	lv_lmeter_set_value(driveTempMeter, temp);
	lv_label_set_text(driveTempLabel, str);
	lv_obj_align(driveTempMeter, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);
	lv_obj_align(driveTempLabel, NULL, LV_ALIGN_CENTER, 0, 0);
}
*/
void display(void) {
	lv_task_handler();
	lv_tick_inc(5);
}

void runSelectedAuto(void) {
	printf("RAN\n");
	printf("%d\n", selectedProgram);
	printf("%d\n", autoType);
	printf("%d\n", AUTONOMOUS_LEFT);
	printf("%d\n", AUTONOMOUS_RIGHT);
	if (selectedProgram == DO_NOT_RUN) {
		return;
	}
	switch (autoType) {
		case AUTONOMOUS_LEFT:
			printf("LEFT\n");
			leftScripts[selectedProgram]();
			break;
		case AUTONOMOUS_RIGHT:
			printf("RIGHT\n");
			rightScripts[selectedProgram]();
			break;
		case AUTONOMOUS_SKILLS:
			printf("SKILLS\n");
			skillsScripts[selectedProgram]();
			break;
	}
}

