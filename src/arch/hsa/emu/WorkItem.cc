/*
 *  Multi2Sim
 *  Copyright (C) 2014  Yifan Sun (yifansun@coe.neu.edu)
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

#include "WorkItem.h"

namespace HSA
{

WorkItem::WorkItem()
{
	emu = Emu::getInstance();

	pid = emu->getPid();	

	emu->loader_debug << "WorkItem " << pid << " created\n";
	emu->isa_debug    << "WorkItem " << pid << " created\n";

}


WorkItem::~WorkItem()
{
}


void WorkItem::Execute()
{
	//std::cout << "In WorkItem::Execute\n";
	BrigInstEntry inst(pc, ProgramLoader::getInstance()->getBinary());

	// Put the inst to perform to isa debug file
	emu->isa_debug << inst;

	// Get the function according to the opcode and perform the inst
	int opcode = inst.getOpcode();
	ExecuteInstFn fn = WorkItem::execute_inst_fn[opcode];
	(this->*fn)();

	// move pc register to next function
	pc = inst.next();
}


}  // namespace HSA
