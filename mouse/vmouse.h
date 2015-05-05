/*
 *  Virtual mouse driver - Alessandro Pira <writeme@alessandropira.org>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __VMOUSE_H
#define __VMOUSE_H

// Defines for scancodes, defaults to keypad keys
#define VMOUSE_SC_UL 0x47 // Keypad 7
#define VMOUSE_SC_UP 0x48 // Keypad 8
#define VMOUSE_SC_UR 0x49 // Keypad 9

#define VMOUSE_SC_LF 0x4B // Keypad 4
#define VMOUSE_SC_RG 0x4D // Keypad 6

#define VMOUSE_SC_DL 0x4F // Keypad 1
#define VMOUSE_SC_DN 0x50 // Keypad 2
#define VMOUSE_SC_DR 0x51 // Keypad 3

#define VMOUSE_SC_B1 0x52 // Keypad 0
#define VMOUSE_SC_B2 0x53 // Keypad .

// If VMOUSE_CONTROL key is not pressed virt_mouse is not active
#define VMOUSE_SC_CONTROL 0x1D // Left CTRL

// Minumum cursor speed
#define STEP 2

// Maximum cursor speed increase (actually, maximum speed is STEP+MAX_ACCEL)
#define MAX_ACCEL 30

// undefine to completely disable logging
#define LOG

// define to turn on debug logging
#undef VERBOSE

#endif // __VMOUSE_H
