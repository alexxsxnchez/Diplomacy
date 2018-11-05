#include <node.h>
#include <v8.h>

using namespace v8;

struct Work {
	//uv_work_t request;
	//Persistent<Function> callback;
};

void processMoves(const FunctionCallbackInfo<Value>& args) {
	Isolate * isolate = args.GetIsolate();
/*
	Work * work = new Work();
	work->request.data = work;
*/	
	


	args.GetReturnValue().Set(Undefined(isolate));
}

void unpackMoves(Isolate * isolate, const FunctionCallbackInfo<Value>& args) {
	
	Local<Object> moves = Local<Object>::Cast(args[0]);
	Local<Array> keys = moves->GetOwnPropertyNames();
	Local<Context> context = isolate->GetCurrentContext();
    unsigned int numMoves = keys->Length();
    for (unsigned int i = 0; i < numMoves; i++) {
    	Local<Value> firstLoc = keys->Get(i);
    	Local<Object> move = Local<Object>::Cast(moves->Get(context, firstLoc).ToLocalChecked());
    	Local<Object> unit = Local<Object>::Cast(move->Get(String::NewFromUtf8(isolate, "unit")));
    	Local<Value> unitType = unit->Get(String::NewFromUtf8(isolate, "type"));
    	Local<Value> nation = unit->Get(String::NewFromUtf8(isolate, "nation"));
    	Local<Value> moveType = move->Get(String::NewFromUtf8(isolate, "type"));
    	Local<Value> secondLoc = move->Get(String::NewFromUtf8(isolate, "secondLoc"));
    	Local<Value> thirdLoc = move->Get(String::NewFromUtf8(isolate, "thirdLoc"));
    	
    	
    }

}

void init(Handle <Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(exports, "processMoves", processMoves);
}

NODE_MODULE(processMoves, init)
