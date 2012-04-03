/*
 *  Multi2Sim Tools
 *  Copyright (C) 2011  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <visual-private.h>


/*
 * Public Functions
 */

struct visual_mod_access_t *visual_mod_access_create(char *name)
{
	struct visual_mod_access_t *access;

	/* Allocate */
	access = calloc(1, sizeof(struct visual_mod_access_t));
	if (!access)
		fatal("%s: out of memory", __FUNCTION__);

	/* Initialize */
	access->name = str_set(access->name, name);

	/* Return */
	return access;
}


void visual_mod_access_free(struct visual_mod_access_t *access)
{
	str_free(access->name);
	str_free(access->state);
	free(access);
}


void visual_mod_access_set_state(struct visual_mod_access_t *access, char *state)
{
	access->state = str_set(access->state, state);
	access->state_update_cycle = state_file_get_cycle(visual_state_file);
}
