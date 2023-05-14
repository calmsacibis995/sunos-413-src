/*
 * prop_ui.c - User interface object initialization functions.
 * This file was generated by `gxv' from `prop.G'.
 * DO NOT EDIT BY HAND.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/icon_load.h>
#include <xview/panel.h>
#include <xview/scrollbar.h>
#include <xview/svrimage.h>
#include <xview/termsw.h>
#include <xview/text.h>
#include <xview/tty.h>
#include "prop_ui.h"

/*
 * Initialize an instance of object `prop'.
 */
prop_prop_objects *
prop_prop_objects_initialize(ip, owner)
	prop_prop_objects	*ip;
	Xv_opaque	owner;
{
	if (!ip && !(ip = (prop_prop_objects *) calloc(1, sizeof (prop_prop_objects))))
		return (prop_prop_objects *) NULL;
	if (!ip->prop)
		ip->prop = prop_prop_prop_create(ip, owner);
	if (!ip->panel1)
		ip->panel1 = prop_prop_panel1_create(ip, ip->prop);
	if (!ip->track_list)
		ip->track_list = prop_prop_track_list_create(ip, ip->panel1);
	if (!ip->mem_list)
		ip->mem_list = prop_prop_mem_list_create(ip, ip->panel1);
	if (!ip->mode_choice)
		ip->mode_choice = prop_prop_mode_choice_create(ip, ip->panel1);
	if (!ip->track_label)
		ip->track_label = prop_prop_track_label_create(ip, ip->panel1);
	if (!ip->track_mess)
		ip->track_mess = prop_prop_track_mess_create(ip, ip->panel1);
	if (!ip->time_label)
		ip->time_label = prop_prop_time_label_create(ip, ip->panel1);
	if (!ip->time_mess)
		ip->time_mess = prop_prop_time_mess_create(ip, ip->panel1);
	if (!ip->guage1)
		ip->guage1 = prop_prop_guage1_create(ip, ip->panel1);
	if (!ip->guage_choice)
		ip->guage_choice = prop_prop_guage_choice_create(ip, ip->panel1);
	if (!ip->mess1)
		ip->mess1 = prop_prop_mess1_create(ip, ip->panel1);
	if (!ip->leftbal_slider)
		ip->leftbal_slider = prop_prop_leftbal_slider_create(ip, ip->panel1);
	if (!ip->rightbal_slider)
		ip->rightbal_slider = prop_prop_rightbal_slider_create(ip, ip->panel1);
	if (!ip->idle_choice)
		ip->idle_choice = prop_prop_idle_choice_create(ip, ip->panel1);
	if (!ip->idle_item)
		ip->idle_item = prop_prop_idle_item_create(ip, ip->panel1);
	return ip;
}

/*
 * Create object `prop' in the specified instance.
 */
Xv_opaque
prop_prop_prop_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void	prop_done();
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, ip,
		XV_WIDTH, 397,
		XV_HEIGHT, 484,
		XV_LABEL, "CD Player - Properties",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, FALSE,
		FRAME_SHOW_RESIZE_CORNER, FALSE,
		FRAME_CMD_PUSHPIN_IN, FALSE,
		FRAME_DONE_PROC, prop_done,
		0);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, 0);
	return obj;
}

/*
 * Create object `panel1' in the specified instance.
 */
Xv_opaque
prop_prop_panel1_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, TRUE,
		0);
	return obj;
}

/*
 * Create object `track_list' in the specified instance.
 */
Xv_opaque
prop_prop_track_list_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern int		track_list_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 32,
		XV_Y, 16,
		PANEL_LIST_WIDTH, 125,
		XV_HEIGHT, 217,
		PANEL_LABEL_STRING, "Tracks",
		PANEL_LAYOUT, PANEL_VERTICAL,
		PANEL_LIST_DISPLAY_ROWS, 10,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, TRUE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, track_list_notify,
		0);
	return obj;
}

/*
 * Create object `mem_list' in the specified instance.
 */
Xv_opaque
prop_prop_mem_list_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern int		mem_list_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 224,
		XV_Y, 16,
		PANEL_LIST_WIDTH, 125,
		XV_HEIGHT, 217,
		PANEL_LABEL_STRING, "Memory Play List",
		PANEL_LAYOUT, PANEL_VERTICAL,
		PANEL_LIST_DISPLAY_ROWS, 10,
		PANEL_READ_ONLY, FALSE,
		PANEL_CHOOSE_ONE, TRUE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, mem_list_notify,
		0);
	return obj;
}

/*
 * Create object `mode_choice' in the specified instance.
 */
Xv_opaque
prop_prop_mode_choice_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void		play_mode_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 47,
		XV_Y, 247,
		XV_WIDTH, 270,
		XV_HEIGHT, 21,
		PANEL_VALUE_X, 128,
		PANEL_VALUE_Y, 247,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_STRINGS,
			"Normal",
			"Memory",
			"Shuffle",
			0,
		PANEL_LABEL_STRING, "Play Mode:",
		PANEL_NOTIFY_PROC, play_mode_notify,
		0);
	return obj;
}

/*
 * Create object `track_label' in the specified instance.
 */
Xv_opaque
prop_prop_track_label_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 69,
		XV_Y, 278,
		XV_WIDTH, 50,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "Tracks:",
		PANEL_LABEL_BOLD, TRUE,
		0);
	return obj;
}

/*
 * Create object `track_mess' in the specified instance.
 */
Xv_opaque
prop_prop_track_mess_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 126,
		XV_Y, 279,
		XV_WIDTH, 16,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "00",
		PANEL_LABEL_BOLD, FALSE,
		0);
	return obj;
}

/*
 * Create object `time_label' in the specified instance.
 */
Xv_opaque
prop_prop_time_label_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 25,
		XV_Y, 303,
		XV_WIDTH, 94,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "Playing Time:",
		PANEL_LABEL_BOLD, TRUE,
		0);
	return obj;
}

/*
 * Create object `time_mess' in the specified instance.
 */
Xv_opaque
prop_prop_time_mess_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 126,
		XV_Y, 303,
		XV_WIDTH, 35,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "00:00",
		PANEL_LABEL_BOLD, FALSE,
		0);
	return obj;
}

/*
 * Create object `guage1' in the specified instance.
 */
Xv_opaque
prop_prop_guage1_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_GAUGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 212,
		XV_Y, 320,
		XV_WIDTH, 116,
		XV_HEIGHT, 41,
		PANEL_VALUE_X, 212,
		PANEL_VALUE_Y, 337,
		PANEL_GAUGE_WIDTH, 120,
		PANEL_LAYOUT, PANEL_VERTICAL,
		PANEL_DIRECTION, PANEL_HORIZONTAL,
		PANEL_SHOW_RANGE, TRUE,
		PANEL_MIN_VALUE, 0,
		PANEL_MAX_VALUE, 100,
		PANEL_TICKS, 0,
		0);
	return obj;
}

/*
 * Create object `guage_choice' in the specified instance.
 */
Xv_opaque
prop_prop_guage_choice_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void		guage_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE, PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 2,
		XV_Y, 331,
		XV_WIDTH, 194,
		XV_HEIGHT, 21,
		PANEL_VALUE_X, 127,
		PANEL_VALUE_Y, 331,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_STRINGS,
			"Track",
			"Total",
			0,
		PANEL_LABEL_STRING, "Time Remaining:",
		PANEL_NOTIFY_PROC, guage_notify,
		0);
	return obj;
}

/*
 * Create object `mess1' in the specified instance.
 */
Xv_opaque
prop_prop_mess1_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 60,
		XV_Y, 364,
		XV_WIDTH, 58,
		XV_HEIGHT, 13,
		PANEL_LABEL_STRING, "Balance:",
		PANEL_LABEL_BOLD, TRUE,
		0);
	return obj;
}

/*
 * Create object `leftbal_slider' in the specified instance.
 */
Xv_opaque
prop_prop_leftbal_slider_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void		leftbal_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_SLIDER,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 87,
		XV_Y, 383,
		XV_WIDTH, 192,
		XV_HEIGHT, 20,
		PANEL_VALUE_X, 126,
		PANEL_VALUE_Y, 383,
		PANEL_SLIDER_WIDTH, 80,
		PANEL_LABEL_STRING, "Left:",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_DIRECTION, PANEL_HORIZONTAL,
		PANEL_SHOW_RANGE, TRUE,
		PANEL_SHOW_VALUE, FALSE,
		PANEL_MIN_VALUE, 0,
		PANEL_MAX_VALUE, 10,
		PANEL_TICKS, 10,
		PANEL_NOTIFY_PROC, leftbal_notify,
		0);
	return obj;
}

/*
 * Create object `rightbal_slider' in the specified instance.
 */
Xv_opaque
prop_prop_rightbal_slider_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void		rightbal_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_SLIDER,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 77,
		XV_Y, 403,
		XV_WIDTH, 201,
		XV_HEIGHT, 20,
		PANEL_VALUE_X, 125,
		PANEL_VALUE_Y, 403,
		PANEL_SLIDER_WIDTH, 80,
		PANEL_LABEL_STRING, "Right:",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_DIRECTION, PANEL_HORIZONTAL,
		PANEL_SHOW_RANGE, TRUE,
		PANEL_SHOW_VALUE, FALSE,
		PANEL_MIN_VALUE, 0,
		PANEL_MAX_VALUE, 10,
		PANEL_TICKS, 10,
		PANEL_NOTIFY_PROC, rightbal_notify,
		0);
	return obj;
}

/*
 * Create object `idle_choice' in the specified instance.
 */
Xv_opaque
prop_prop_idle_choice_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern void		idle_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TOGGLE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 11,
		XV_Y, 446,
		XV_WIDTH, 85,
		XV_HEIGHT, 25,
		PANEL_VALUE_X, 20,
		PANEL_VALUE_Y, 446,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOICE_STRINGS,
			"Idle Mode",
			0,
		PANEL_NOTIFY_PROC, idle_notify,
		0);
	return obj;
}

/*
 * Create object `idle_item' in the specified instance.
 */
Xv_opaque
prop_prop_idle_item_create(ip, owner)
	caddr_t		*ip;
	Xv_opaque	owner;
{
	extern Panel_setting	idle_value_notify();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_NUMERIC_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 138,
		XV_Y, 449,
		XV_WIDTH, 232,
		XV_HEIGHT, 15,
		PANEL_LABEL_STRING, "Seconds until idle:",
		PANEL_VALUE_X, 273,
		PANEL_VALUE_Y, 449,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_VALUE_DISPLAY_LENGTH, 8,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_MAX_VALUE, 9999,
		PANEL_MIN_VALUE, 5,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, idle_value_notify,
		0);
	return obj;
}
