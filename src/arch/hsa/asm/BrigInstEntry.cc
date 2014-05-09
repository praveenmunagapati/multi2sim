#include <sstream>
#include "BrigInstEntry.h"
#include "BrigOperandEntry.h"
#include "Asm.h"

namespace HSA{


const char *BrigInstEntry::opcode2str(InstOpcode opcode) const
{
	Asm *as = Asm::getInstance();
	const struct InstInfo *inst_info = as->getInstInfo();
	return inst_info[opcode].fmt_str;
}

template <typename BrigInst>
const char *BrigInstEntry::rounding2str(BrigInst* base) const
{
	unsigned short rounding;
	//unsigned short defaultRounding;
	rounding = base->modifier.allBits;
	rounding &= 15;
	//FIXME compare rouding with deaultRounding	
	return BrigInstEntry::rounding_to_str_map.MapValue(rounding);
}

misc::StringMap BrigInstEntry::rounding_to_str_map = 
{
	{"", 0},
	{"", 1},
	{"zero", 2},
	{"up", 3},
	{"down", 4},
	{"neari", 5},
	{"zeroi", 6},
	{"upi", 7},
	{"downi", 8},
	{"neari_sat", 9},
	{"zeroi_sat", 10},
	{"upi_sat", 11},
	{"downi_sat", 12}
};

const char *BrigInstEntry::modifier2str(unsigned short modifier) const
{
	unsigned short ftz = modifier & 16;
	if(ftz)
	{
		return "ftz";
	}
	return "";
}

misc::StringMap BrigInstEntry::pack_to_str_map = 
{
	{"", 0},
	{"pp", 1},
	{"ps", 2},
	{"sp", 3},
	{"ss", 4},
	{"s", 5},
	{"p", 6},
	{"pp_sat", 7},
	{"ps_sat", 8},
	{"sp_sat", 9},
	{"ss_sat", 10},
	{"s_sat", 11},
	{"p_sat", 12}
};

const char *BrigInstEntry::pack2str(unsigned char pack) const
{
	return BrigInstEntry::pack_to_str_map.MapValue(pack);
}

misc::StringMap BrigInstEntry::v_to_str_map = 
{
	{"v2", 2},
	{"v3", 3},
	{"v4", 4}
};

const char *BrigInstEntry::operandV2str(char *o) const
{
	struct BrigOperandBase *operand = (struct BrigOperandBase *)o;
	switch(operand->kind)
	{
	case BRIG_OPERAND_REG_VECTOR:
	{
		struct BrigOperandRegVector *v = (struct BrigOperandRegVector *)o;
		return BrigInstEntry::v_to_str_map.MapValue(v->regCount);		
	}
	case BRIG_OPERAND_REG:
		return "";
		break;
	case BRIG_OPERAND_IMMED:
		return "";
		break;
	default:
		return "<Invalid vX operand>";
	}
	
}

const char *BrigInstEntry::v2str(char* i) const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)i;
	switch(inst->opcode)
	{
	case BRIG_OPCODE_LD:
	case BRIG_OPCODE_ST:
	case BRIG_OPCODE_EXPAND:
	case BRIG_OPCODE_RDIMAGE:
	case BRIG_OPCODE_LDIMAGE:
	case BRIG_OPCODE_STIMAGE:
		return operandV2str(getOperand(0));
		break;
	case BRIG_OPCODE_COMBINE:
		return operandV2str(getOperand(1));
		break;
	default:
		break;
	}
	return "";
}

misc::StringMap BrigInstEntry::width_to_str_map = 
{
	{"", 0},
	{"", 1},
	{"width(2)", 2},
	{"width(4)", 3},
	{"width(8)", 4},
	{"width(16)", 5},
	{"width(32)", 6},
	{"width(64)", 7},
	{"width(128)", 8},
	{"width(256)", 9},
	{"width(512)", 10},
	{"width(1024)", 11},
	{"width(2048)", 12},
	{"width(4096)", 13},
	{"width(8192)", 14},
	{"width(16384)", 15},
	{"width(32768)", 16},
	{"width(65536)", 17},
	{"width(131072)", 18},
	{"width(262144)", 19},
	{"width(524288)", 20},
	{"width(1048576)", 21},
	{"width(2097152)", 22},
	{"width(4194304)", 23},
	{"width(8388608)", 24},
	{"width(16777216)", 25},
	{"width(33554432)", 26},
	{"width(67108864)", 27},
	{"width(134217728)", 28},
	{"width(268435456)", 29},
	{"width(536870912)", 30},
	{"width(1073741824)", 31},
	{"width(2147483648)", 32},
	{"width(WAVESIZE)", 33},
	{"width(ALL)", 34}
};

template<class T>
const char *BrigInstEntry::width2str(T *inst) const
{
	return width_to_str_map.MapValue(inst->width);	
}

misc::StringMap BrigInstEntry::comp_op_to_str_map = 
{
	{"eq", 0},
	{"ne", 1},
	{"lt", 2},
	{"le", 3},
	{"gt", 4},
	{"ge", 5},
	{"equ", 6},
	{"neu", 7},
	{"ltu", 8},
	{"leu", 9},
	{"gtu", 10},
	{"geu", 11},
	{"num", 12},
	{"nan", 13},
	{"seq", 14},
	{"sne", 15},
	{"slt", 16},
	{"sle", 17},
	{"sgt", 18},
	{"sge", 19},
	{"sgeu", 20},
	{"sequ", 21},
	{"sneu", 22},
	{"sltu", 23},
	{"sleu", 24},
	{"snum", 25},
	{"snan", 26},
	{"sgtu", 27}
};

const char *BrigInstEntry::cmpOp2str(unsigned char op) const
{
	return comp_op_to_str_map.MapValue(op);
}

const char *BrigInstEntry::equiv2str(unsigned char val) const
{
	std::stringstream s;
	if(val != 0) s << "equiv(" << (unsigned) val << ")";
	return s.str().c_str();
}

const char *BrigInstEntry::aligned2str(unsigned char modifier) const
{
	unsigned char aligned = modifier & BRIG_MEMORY_ALIGNED;
	if(aligned) return "aligned";
	return "";
}

bool BrigInstEntry::hasType() const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	switch(inst->opcode){
	case BRIG_OPCODE_CBR:
	case BRIG_OPCODE_BRN:
	case BRIG_OPCODE_CALL:
	case BRIG_OPCODE_RET:
	case BRIG_OPCODE_BARRIER:
	case BRIG_OPCODE_INITFBAR:
	case BRIG_OPCODE_JOINFBAR:
	case BRIG_OPCODE_WAITFBAR:
	case BRIG_OPCODE_ARRIVEFBAR:
	case BRIG_OPCODE_LEAVEFBAR:
	case BRIG_OPCODE_RELEASEFBAR:
	case BRIG_OPCODE_SYNC:
	case BRIG_OPCODE_NOP:
		return false;

	case BRIG_OPCODE_GCNMADS:
	case BRIG_OPCODE_GCNMADU:
	case BRIG_OPCODE_GCNMQSAD4:
	case BRIG_OPCODE_GCNREGIONALLOC:
		return false;
	
	default:
		return true;
	}
}

char *BrigInstEntry::getOperand(int i) const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	return BrigOperandEntry::GetOperandBufferByOffset(
			file, inst->operands[i]
		);
}

void BrigInstEntry::dumpOperands(std::ostream &os = std::cout) const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	os << "\t";
	for(int i=0; i<5; i++)
	{
		if(inst->operands[i] == 0 ) return;
		if(i>0) os << ", ";
		BrigOperandEntry op(
			BrigOperandEntry::GetOperandBufferByOffset(file, inst->operands[i]),
			file
		);
		op.Dump(os);
	}
}

BrigInstEntry::BrigInstEntry(char *buf, BrigFile *file)
	:BrigEntry(buf, file)
{
}

int BrigInstEntry::getKind() const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	return inst->kind;
}

int BrigInstEntry::getOpcode() const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	return inst->opcode;
}

BrigInstEntry::DumpInstFn BrigInstEntry::dump_inst_fn[] = 
{
	&BrigInstEntry::DumpInstNone,
	&BrigInstEntry::DumpInstBasic,
	&BrigInstEntry::DumpInstAtomic,
	&BrigInstEntry::DumpInstAtomicImage,
	&BrigInstEntry::DumpInstBar,
	&BrigInstEntry::DumpInstBr,
	&BrigInstEntry::DumpInstCmp,
	&BrigInstEntry::DumpInstCvt,
	&BrigInstEntry::DumpInstFbar,
	&BrigInstEntry::DumpInstImage,
	&BrigInstEntry::DumpInstMem,
	&BrigInstEntry::DumpInstAddr,
	&BrigInstEntry::DumpInstMod,
	&BrigInstEntry::DumpInstSeg,
	&BrigInstEntry::DumpInstSourceType
};

void BrigInstEntry::DumpInstNone(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("None", os);
}

void BrigInstEntry::DumpInstBasic(std::ostream &os = std::cout) const
{
	struct BrigInstBasic *inst = (struct BrigInstBasic *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	if(this->hasType()) os << "_" << BrigEntry::type2str(inst->type);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstAtomic(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("Atomic", os);
}

void BrigInstEntry::DumpInstAtomicImage(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("Image", os);
}

void BrigInstEntry::DumpInstCvt(std::ostream &os = std::cout) const
{
	struct BrigInstCvt *inst = (struct BrigInstCvt *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(modifier2str(inst->modifier.allBits), os);
	dump_(rounding2str(inst), os);
	dump_(type2str(inst->type), os);
	dump_(type2str(inst->sourceType), os);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstBar(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("Bar", os);
}

void BrigInstEntry::DumpInstBr(std::ostream &os = std::cout) const
{
	struct BrigInstBr *inst = (struct BrigInstBr *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(width2str(inst), os);
	dump_(modifier2str(inst->modifier.allBits), os);
	dump_(rounding2str(inst), os);
	if(this->hasType()) dump_(type2str(inst->type));
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstCmp(std::ostream &os = std::cout) const
{
	struct BrigInstCmp *inst = (struct BrigInstCmp *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(cmpOp2str(inst->compare), os);
	dump_(modifier2str(inst->modifier.allBits), os);
	dump_(rounding2str(inst), os);
	dump_(pack2str(inst->pack), os);
	dump_(type2str(inst->type), os);
	dump_(type2str(inst->sourceType), os);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstFbar(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("Fbar", os);
}

void BrigInstEntry::DumpInstImage(std::ostream &os = std::cout) const
{
	this->DumpInstUnsupported("Image", os);
}

void BrigInstEntry::DumpInstMem(std::ostream &os = std::cout) const
{
	struct BrigInstMem *inst = (struct BrigInstMem *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(v2str(base), os);
	dump_(width2str(inst), os);
	dump_(BrigEntry::seg2str(inst->segment), os);
	dump_(aligned2str(inst->modifier.allBits), os);
	dump_(BrigEntry::sem2str(inst->modifier.allBits), os);
	dump_(equiv2str(inst->equivClass), os);
	dump_(type2str(inst->type), os);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstAddr(std::ostream &os = std::cout) const
{
	struct BrigInstAddr *inst = (struct BrigInstAddr *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(BrigEntry::seg2str(inst->segment));
	dump_(BrigEntry::type2str(inst->type));
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstMod(std::ostream &os = std::cout) const
{
	struct BrigInstMod *inst = (struct BrigInstMod *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(modifier2str(inst->modifier.allBits), os);
	dump_(rounding2str(inst), os);
	dump_(pack2str(inst->pack), os);
	if(this->hasType()) os << "_" << BrigEntry::type2str(inst->type);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstSeg(std::ostream &os = std::cout) const
{
	struct BrigInstSeg *inst = (struct BrigInstSeg *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	dump_(BrigEntry::seg2str(inst->segment), os);
	dump_(BrigEntry::type2str(inst->type), os);
	dump_(BrigEntry::type2str(inst->sourceType), os);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstSourceType(std::ostream &os = std::cout) const
{
	struct BrigInstSourceType *inst = (struct BrigInstSourceType *)base;
	os << this->opcode2str((InstOpcode)inst->opcode);
	
	dump_(v2str(base), os);
	os << "_" << BrigEntry::type2str(inst->type);
	os << "_" << BrigEntry::type2str(inst->sourceType);
	this->dumpOperands(os);
	os << "\n";
}

void BrigInstEntry::DumpInstUnsupported( 
		const char *kind, 
		std::ostream &os = std::cout
	) const
{
	struct BrigInstBase *inst = (struct BrigInstBase *)base;
	os << "<unsupported kind:" << kind 
		<< "(" << inst->kind << ") " 
		<< "opcode:" << inst->opcode << " >\n";
}
}//end namespace