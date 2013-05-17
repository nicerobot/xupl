#include "xupl.h"

int main (int argc, char *argv[]) {

	xupl*_=xupl_init(argc,argv);
	_->parse(_);
	_->print(_);
	_->done(_);

}
