/*
 *  Multi2Sim Tools
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
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

#include <assert.h>
#include <stdio.h>

#include <lib/util/debug.h>
#include <lib/util/list.h>
#include <lib/util/hash-table.h>
#include <lib/util/misc.h>
#include <lib/mhandle/mhandle.h>
#include <visual/common/state.h>
#include <visual/common/trace.h>

#include "buffer.h"
#include "node.h"
#include "net.h"
#include "net-system.h"


struct vi_net_buffer_t *vi_net_buffer_create (struct vi_trace_line_t *trace_line,
		enum vi_buffer_direction_t buffer_direction)
{
	struct vi_net_buffer_t *buffer;

	/* Return */
	buffer = xcalloc(1, sizeof(struct vi_net_buffer_t));

	/* Get network Name */
	char *name;
	struct vi_net_t *net;
	struct vi_net_node_t *node;

	/* Get the network name */
	name = vi_trace_line_get_symbol(trace_line, "net_name");
	net = hash_table_get(vi_net_system->net_table, name);

	assert(net);

	/* Get the Node */
	char *node_name;
	node_name = vi_trace_line_get_symbol(trace_line, "node_name");

	node = hash_table_get(net->node_table, node_name);
	assert(node);
	buffer->node = node;

	/* Get the buffer by index and direction */
	char *buffer_name;
	buffer_name = vi_trace_line_get_symbol(trace_line, "buffer_name");
	buffer->name = xstrdup(buffer_name);

	buffer->direction = buffer_direction;

	if (buffer_direction == vi_buffer_dir_input)
		hash_table_insert(node->input_buffer_list, buffer->name, buffer);

	else if (buffer_direction == vi_buffer_dir_output)
		hash_table_insert(node->output_buffer_list, buffer->name, buffer);

	else
		panic("%s: buffer direction is not valid", __FUNCTION__);

	/* Set buffer size */
	int buffer_size ;
	buffer_size = vi_trace_line_get_symbol_int(trace_line, "buffer_size");
	buffer->size = buffer_size;

	/* Set buffer connection Type */
	int connection_type;
	connection_type = vi_trace_line_get_symbol_int(trace_line, "buffer_type");
	buffer->connection_type = connection_type;

	/* Set buffer Link */
	if (buffer->connection_type == vi_buffer_link)
	{
		char *link_name;
		struct vi_net_link_t *link;

		link_name = vi_trace_line_get_symbol(trace_line, "connection");
		link = hash_table_get(net->link_table, link_name);
		buffer->link = link;

	}
	else if ((buffer->connection_type == vi_buffer_bus ) ||
			(buffer->connection_type == vi_buffer_photonic ))
	{
		char *bus_node_name;
		struct vi_net_node_t *bus_node;
		bus_node_name = vi_trace_line_get_symbol(trace_line, "connection");
		bus_node = hash_table_get(net->node_table, bus_node_name);
		assert(bus_node && ((bus_node->type == vi_net_node_bus) || (bus_node->type == vi_net_node_photonic)));

		if (buffer->direction == vi_buffer_dir_input)
			hash_table_insert(bus_node->dst_buffer_list, buffer->name, buffer);
		else if (buffer->direction == vi_buffer_dir_output)
			hash_table_insert(bus_node->src_buffer_list, buffer->name, buffer);

	}

	return buffer;
}

void vi_net_buffer_free (struct vi_net_buffer_t *buffer)
{
	free(buffer);
}