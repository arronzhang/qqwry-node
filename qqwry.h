#ifndef QQWRY_H
#define QQWRY_H

#include <node.h>
#include <v8.h>

#include <string.h>
#include <errno.h>
#include <iconv.h>

extern "C" {
#include "libqqwry.h"
}

using namespace node;
using namespace v8;

class qqwry: ObjectWrap
{
	public:
		static void Initialize(Handle<Object> target);
	protected:
		static Persistent<FunctionTemplate> constructor_template;
		static Handle<Value> New (const Arguments& args);
		static Handle<Value> Query (const Arguments& args);
		static Handle<Value> Close (const Arguments& args);

		qqwry () : ObjectWrap() {
		};

		~qqwry (){};

	private:
		static int init(char *path);
		static int query(char *ip, char *loc);
		static void destroy();
};

#endif
