/**
 *
 *
 *
 *  @author Sam Shull <http://samshull.blogspot.com/>
 *  @version 0.1
 *
 *  @copyright Copyright (c) 2009 Sam Shull <http://samshull.blogspot.com/>
 *  @license <http://www.opensource.org/licenses/mit-license.html>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  CHANGES:
 */

#include "./node-proxy.h"

namespace v8 {  // this was much easier
  // fundamental traps
Persistent<String> NodeProxy::getOwnPropertyDescriptor;
Persistent<String> NodeProxy::getPropertyDescriptor;
Persistent<String> NodeProxy::getOwnPropertyNames;
Persistent<String> NodeProxy::getPropertyNames;
Persistent<String> NodeProxy::defineProperty;
Persistent<String> NodeProxy::delete_;
Persistent<String> NodeProxy::fix;
  // derived traps
Persistent<String> NodeProxy::has;
Persistent<String> NodeProxy::hasOwn;
Persistent<String> NodeProxy::get;
Persistent<String> NodeProxy::set;
Persistent<String> NodeProxy::enumerate;
Persistent<String> NodeProxy::keys;
  // string identifiers
Persistent<String> NodeProxy::callTrap;
Persistent<String> NodeProxy::constructorTrap;
Persistent<String> NodeProxy::value;
Persistent<String> NodeProxy::writable;
Persistent<String> NodeProxy::enumerable;
Persistent<String> NodeProxy::configurable;
Persistent<String> NodeProxy::name;
Persistent<String> NodeProxy::trapping;
Persistent<String> NodeProxy::sealed;
Persistent<String> NodeProxy::frozen;
Persistent<String> NodeProxy::extensible;
Persistent<String> NodeProxy::seal;
Persistent<String> NodeProxy::freeze;
Persistent<String> NodeProxy::preventExtensions;
Persistent<String> NodeProxy::isTrapping;
Persistent<String> NodeProxy::isSealed;
Persistent<String> NodeProxy::isFrozen;
Persistent<String> NodeProxy::isExtensible;
Persistent<String> NodeProxy::isProxy;
Persistent<String> NodeProxy::hidden;
Persistent<String> NodeProxy::hiddenPrivate;

Persistent<ObjectTemplate> NodeProxy::ObjectCreator;
Persistent<ObjectTemplate> NodeProxy::FunctionCreator;

/**
 *
 *
 *
 *
 */
NodeProxy::NodeProxy() {
}

/**
 *
 *
 *
 *
 */
NodeProxy::~NodeProxy() {
}

/**
 *  Used for creating a shallow copy of an object
 *
 *
 *  @param mixed
 *  @returns mixed
 *  @throws Error
 */
Handle<Value> NodeProxy::Clone(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    return THREXC("clone requires at least one (1) argument.");
  }

  if (args[0]->IsString()) {
    return args[0]->ToObject()->Clone()->ToString();

  } else if (args[0]->IsBoolean()) {
    return args[0]->ToObject()->Clone()->ToBoolean();

  } else if (args[0]->IsNumber()
        || args[0]->IsInt32()
        || args[0]->IsUint32()) {
    return args[0]->ToObject()->Clone()->ToNumber();

  } else if (args[0]->IsArray()) {
    return Local<Array>::Cast(args[0]->ToObject()->Clone());

  } else if (args[0]->IsDate()) {
    return Local<Date>::Cast(args[0]->ToObject()->Clone());

  } else if (args[0]->IsFunction()) {
    return Local<Function>::Cast(args[0])->Clone();

  } else if (args[0]->IsNull()) {
    return Local<Value>::New(Null());

  } else if (args[0]->IsUndefined()) {
    return Local<Value>::New(Undefined());

  } else if (args[0]->IsObject()) {
    return args[0]->ToObject()->Clone();
  }

  return THREXC("clone cannot determine the type of the argument.");
}

/**
 *  Set or Retrieve the value of a hidden
 *  property on a given object
 *  Passing two arguments to this function
 *  returns the value of the hidden property
 *  While passing three arguments to this function
 *  results in the setting of the hidden property
 *  and returns a Boolean value indicating successful
 *  setting of value
 *
 *  @param Object
 *  @param String name
 *  @param mixed value - optional
 *  @returns mixed
 *  @throws Error
 */
Handle<Value> NodeProxy::Hidden(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    return THREXC("hidden requires at least two (2) arguments.");
  }

  Local<Object> obj = args[0]->ToObject();

  if (args.Length() < 3) {
    return obj->GetHiddenValue(
          String::Concat(NodeProxy::hidden,
                   args[1]->ToString()));
  }

  return Boolean::New(
        obj->SetHiddenValue(String::Concat(NodeProxy::hidden,
                           args[1]->ToString()),
        args[2]));
}

/**
 *  Set the prototype of an object
 *
 *  @param Object
 *  @param Object
 *  @returns Boolean
 *  @throws Error
 */
Handle<Value> NodeProxy::SetPrototype(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    return THREXC("setPrototype requires at least two (2) arguments.");
  }
  return Boolean::New(args[0]->ToObject()->SetPrototype(args[1]));
}

/**
 *  Determine if an Object was created by Proxy
 *
 *  @param Object
 *  @returns Boolean
 */
Handle<Value> NodeProxy::IsProxy(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    return THREXC("isProxy requires at least one (1) argument.");
  }

  Local<Object> obj = args[0]->ToObject();

  if (obj->InternalFieldCount() > 0) {
    Local<Value> temp = obj->GetInternalField(0);

    return Boolean::New(!temp.IsEmpty() && temp->IsObject());
  }

  return False();
}

/**
 *  Create an object that has ProxyHandler intercepts attached and
 *  optionally implements the prototype of another object
 *
 *  * ProxyHandler intercepts override the property handlers for any
 *  * given prototype. So, the ProxyHandler will be invoked for access
 *  * to the prototype's properties as well
 *
 *  @param ProxyHandler - @see NodeProxy::ValidateProxyHandler
 *  @param Object - optional, the prototype object to implement
 *  @returns Object
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::Create(const Arguments& args) {
  HandleScope scope;

  Local<Object> proxyHandler;

  if (args.Length() < 1) {
    return THREXC("create requires at least one (1) argument.");
  }

  if (!args[0]->IsObject()) {
    return THR_TYPE_ERROR(
        "create requires the first argument to be an Object.");
  }

  proxyHandler = args[0]->ToObject();

  if (args.Length() > 1 && !args[1]->IsObject()) {
    return THR_TYPE_ERROR(
        "create requires the second argument to be an Object.");
  }

  // manage locking states
  proxyHandler->SetHiddenValue(NodeProxy::trapping, True());
  proxyHandler->SetHiddenValue(NodeProxy::extensible, True());
  proxyHandler->SetHiddenValue(NodeProxy::sealed, False());
  proxyHandler->SetHiddenValue(NodeProxy::frozen, False());

  Local<Object> instance = ObjectCreator->NewInstance();

  instance->SetInternalField(0, proxyHandler);

  if (args.Length() > 1) {
    instance->SetPrototype(args[1]);
  }

  return scope.Close(instance);
}

/**
 *  Create a function that has ProxyHandler intercepts attached and
 *  sets a call trap function for invokation as well as an optional
 *  constructor trap
 *
 *
 *  @param ProxyHandler - @see NodeProxy::ValidateProxyHandler
 *  @param Function - call trap
 *  @param Function - optional, constructor trap
 *  @returns Function
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::CreateFunction(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    return THREXC("createFunction requires at least two (2) arguments.");
  }

  if (!args[0]->IsObject()) {
    return THR_TYPE_ERROR(
      "createFunction requires the first argument to be an Object.");
  }
  Local<Object> proxyHandler = args[0]->ToObject();

  if (!args[1]->IsFunction()) {
    return THR_TYPE_ERROR(
      "createFunction requires the second argument to be a Function.");
  }

  if (args.Length() > 2 && !args[2]->IsFunction()) {
    return THR_TYPE_ERROR(
      "createFunction requires the second argument to be a Function.");
  }

  proxyHandler->SetHiddenValue(NodeProxy::callTrap, args[1]);
  proxyHandler->SetHiddenValue(NodeProxy::constructorTrap,
                 args.Length() > 2
                 ? args[2]
                 : Local<Value>::New(Undefined()));
  // manage locking states
  proxyHandler->SetHiddenValue(NodeProxy::trapping, True());
  proxyHandler->SetHiddenValue(NodeProxy::extensible, True());
  proxyHandler->SetHiddenValue(NodeProxy::sealed, False());
  proxyHandler->SetHiddenValue(NodeProxy::frozen, False());


  Local<Object> fn = FunctionCreator->NewInstance();
  fn->SetPrototype(args[1]->ToObject()->GetPrototype());

  fn->SetInternalField(0, proxyHandler);

  return scope.Close(fn);
}

/**
 *  Used as a handler for freeze, seal, and preventExtensions
 *  to lock the state of a Proxy created object
 *
 *  @param Object
 *  @returns Boolean
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::Freeze(const Arguments& args) {
  HandleScope scope;

  Local<String> name = args.Callee()->GetName()->ToString();

  if (args.Length() < 1) {
    return THREXCW(String::Concat(name,
             String::New(" requires at least one (1) argument.")));
  }

  Local<Object> obj = args[0]->ToObject();

  if (obj->InternalFieldCount() < 1) {
    return THR_TYPE_ERROR(
      "Locking functions expect first "
      "argument to be intialized by Proxy");
  }

  Local<Value> hide = obj->GetInternalField(0);

  if (hide.IsEmpty() || !hide->IsObject()) {
    return THR_TYPE_ERROR(
      "Locking functions expect first "
      "argument to be intialized by Proxy");
  }

  Local<Object> handler = hide->ToObject();

  // if the object already meets the requirements of the function call
  if (name->Equals(NodeProxy::freeze)) {
    if (handler->GetHiddenValue(NodeProxy::frozen)->BooleanValue()) {
      return True();
    }

  } else if (name->Equals(NodeProxy::seal)) {
    if (handler->GetHiddenValue(NodeProxy::sealed)->BooleanValue()) {
      return True();
    }

  } else if (name->Equals(NodeProxy::preventExtensions)) {
    if (handler->GetHiddenValue(NodeProxy::extensible)->BooleanValue()) {
      return True();
    }
  }

  // if this object is not trapping, just set the appropriate parameters
  if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
    if (name->Equals(NodeProxy::freeze)) {
      handler->SetHiddenValue(NodeProxy::frozen, True());
      handler->SetHiddenValue(NodeProxy::sealed, True());
      handler->SetHiddenValue(NodeProxy::extensible, False());
      return True();

    } else if (name->Equals(NodeProxy::seal)) {
      handler->SetHiddenValue(NodeProxy::sealed, True());
      handler->SetHiddenValue(NodeProxy::extensible, False());
      return True();

    } else if (name->Equals(NodeProxy::preventExtensions)) {
      handler->SetHiddenValue(NodeProxy::extensible, False());
      return True();
    }
  }

  // Harmony Proxy handling of fix
  Local<Function> fix = Local<Function>::Cast(handler->Get(NodeProxy::fix));
  Local<Value> argv[0];

  Local<Value> pieces = fix->Call(args[0]->ToObject(), 0, argv);

  if (pieces.IsEmpty() || !pieces->IsObject()) {
    return THR_TYPE_ERROR("Cannot lock object.");
  }

  Local<Object> parts = pieces->ToObject();

  // set the appropriate parameters
  if (name->Equals(NodeProxy::freeze)) {
    parts->SetHiddenValue(NodeProxy::frozen, True());
    parts->SetHiddenValue(NodeProxy::sealed, True());
    parts->SetHiddenValue(NodeProxy::extensible, False());

  } else if (name->Equals(NodeProxy::seal)) {
    parts->SetHiddenValue(NodeProxy::sealed, True());
    parts->SetHiddenValue(NodeProxy::extensible, False());

  } else if (name->Equals(NodeProxy::preventExtensions)) {
    parts->SetHiddenValue(NodeProxy::extensible, False());
  }

  parts->SetHiddenValue(NodeProxy::trapping, False());

  // overwrite the handler, making handler available for GC
  obj->SetInternalField(0, parts);

  return True();
}

/**
 *  Used as a handler for determining isTrapped,
 *  isFrozen, isSealed, and isExtensible
 *
 *  @param Object
 *  @returns Boolean
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::IsLocked(const Arguments& args) {
  HandleScope scope;

  Local<String> name = args.Callee()->GetName()->ToString();

  if (args.Length() < 1) {
    return THREXCW(String::Concat(name,
             String::New(" requires at least one (1) argument.")));
  }

  Local<Object> arg = args[0]->ToObject();

  if (arg->InternalFieldCount() < 1) {
    return THR_TYPE_ERROR(
       "Locking functions expect first argument "
       "to be intialized by Proxy");
  }

  Local<Value> hide = arg->GetInternalField(0);

  if (hide.IsEmpty() || !hide->IsObject()) {
    return THR_TYPE_ERROR(
      "Locking functions expect first argument "
      "to be intialized by Proxy");
  }

  Local<Object> obj = hide->ToObject();

  if (name->Equals(NodeProxy::isExtensible)) {
    return obj->GetHiddenValue(NodeProxy::extensible)->ToBoolean();

  } else if (name->Equals(NodeProxy::isSealed)) {
    return obj->GetHiddenValue(NodeProxy::sealed)->ToBoolean();

  } else if (name->Equals(NodeProxy::isTrapping)) {
    return obj->GetHiddenValue(NodeProxy::trapping)->ToBoolean();

  } else if (name->Equals(NodeProxy::isFrozen)) {
    return obj->GetHiddenValue(NodeProxy::frozen)->ToBoolean();
  }

  return False();
}

/**
 *  Part of ECMAScript 5, but only for use on
 *  Objects and Functions created by Proxy
 *
 *  @param Object
 *  @param String - the name of the property
 *  @returns PropertyDescriptor
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::GetOwnPropertyDescriptor(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    return THREXC("getOwnPropertyDescriptor requires "
        "at least two (2) arguments.");
  }

  if (!args[1]->IsString() && !args[1]->IsNumber()) {
    return THR_TYPE_ERROR("getOwnPropertyDescriptor requires "
           "the second argument to be a String or a Number.");
  }

  Local<Object> obj = args[0]->ToObject();
  Local<String> name = args[1]->ToString();

  if (obj->InternalFieldCount() < 1) {
    return THR_TYPE_ERROR("getOwnPropertyDescriptor expects "
            "first argument to be intialized by Proxy");
  }

  Local<Value> temp = obj->GetInternalField(0);

  if (temp.IsEmpty() || !temp->IsObject()) {
    return THR_TYPE_ERROR("getOwnPropertyDescriptor expects "
            "first argument to be intialized by Proxy");
  }

  Local<Object> handler = temp->ToObject();

  if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
    return handler->Get(name);
  }

  Local<Function> getOwn =
    Local<Function>::Cast(
       handler->Get(NodeProxy::getOwnPropertyDescriptor));

  Local<Value> argv[1] = {args[1]};
  return getOwn->Call(obj, 1, argv);
}

/**
 *  Part of ECMAScript 5, but only for use on
 *  Objects and Functions created by Proxy
 *
 *  @param Object
 *  @param String - the name of the property
 *  @param PropertyDescriptor
 *  @returns Boolean
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::DefineProperty(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 3) {
    return THREXC("defineProperty requires at least three (3) arguments.");
  }

  if (!args[1]->IsString() && !args[1]->IsNumber()) {
    return THR_TYPE_ERROR("defineProperty requires the "
                "second argument to be a String or a Number.");
  }

  if (!args[2]->IsObject()) {
    return THR_TYPE_ERROR("defineProperty requires the third argument "
            "to be an Object of the type PropertyDescriptor.");
  }

  Local<Object> obj = args[0]->ToObject();

  if (obj->InternalFieldCount() < 1) {
    return THR_TYPE_ERROR("defineProperty expects first "
                "argument to be intialized by Proxy");
  }

  Local<Value> temp = obj->GetInternalField(0);

  if (temp.IsEmpty() || !temp->IsObject()) {
    return THR_TYPE_ERROR("defineProperty expects first argument "
                "to be intialized by Proxy");
  }

  Local<String> name = args[1]->ToString();
  Local<Object> handler = temp->ToObject();

  if (handler->GetHiddenValue(NodeProxy::sealed)->BooleanValue() ||
  !handler->Has(NodeProxy::defineProperty)) {
    return False();
  }

  if (!handler->GetHiddenValue(NodeProxy::extensible)->BooleanValue() &&
        !handler->Has(name)) {
    return False();
  }

  if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
    Local<Object> desc = handler->Get(name)->ToObject();

    if (desc->Get(NodeProxy::configurable)->BooleanValue()) {
      return Boolean::New(
           handler->Set(name, args[2]->ToObject()));
    }
    return False();
  }

  Local<Function> def = Local<Function>::Cast(
                  handler->Get(NodeProxy::defineProperty));

  Local<Value> argv[2] = {args[1], args[2]->ToObject()};

  return def->Call(obj, 2, argv)->ToBoolean();
}

/**
 *  Part of ECMAScript 5, but only for use on
 *  Objects and Functions created by Proxy
 *
 *  @param Object
 *  @param Object - name/PropertyDescriptor pairs
 *  @returns Boolean
 *  @throws Error, TypeError
 */
Handle<Value> NodeProxy::DefineProperties(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    return THREXC("defineProperty requires at least three (3) arguments.");
  }

  if (!args[1]->IsObject()) {
    return THR_TYPE_ERROR("defineProperty requires the third argument "
             "to be an Object of the type PropertyDescriptor.");
  }

  Local<Object> obj = args[0]->ToObject();

  if (obj->InternalFieldCount() < 1) {
    return THR_TYPE_ERROR("defineProperty expects first "
                "argument to be intialized by Proxy");
  }

  Local<Value> temp = obj->GetInternalField(0);

  if (!temp.IsEmpty() && temp->IsObject()) {
    Local<Object> props = args[1]->ToObject();
    Local<Object> handler = temp->ToObject();

    if (handler->GetHiddenValue(NodeProxy::sealed)->BooleanValue()) {
      return False();
    }

    bool extensible = handler->GetHiddenValue(
                  NodeProxy::extensible)->BooleanValue();
    Local<Array> names = props->GetPropertyNames();
    uint32_t i = 0, l = names->Length();

    if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
      for (;i < l; ++i) {
        Local<Object> name = names->CloneElementAt(i);

        if (handler->Has(name->ToString()) &&
            handler->Get(name->ToString())->IsObject()
        ) {
          Local<Object> tempObj =
            handler->Get(name->ToString())->ToObject();

          if (tempObj->Get(NodeProxy::configurable)->BooleanValue()) {
            if (!handler->Set(name->ToString(),
                      props->Get(name->ToString()))) {
              return THREXCW(
                String::Concat(
                  String::New("Unable to define property: "),
                  name->ToString()));
            }
          }
        } else {
          return THREXCW(String::Concat(
                  String::New("Unable to define property: "),
                  name->ToString()));
        }
      }
      return True();
    }

    Local<Function> def =
      Local<Function>::Cast(handler->Get(NodeProxy::defineProperty));

    TryCatch firstTry;
    for (;i < l; ++i) {
      Local<Value> name = names->Get(i);

      if (extensible || obj->Has(name->ToString())) {
    Local<Value> pd = props->Get(name->ToString());
    Local<Value> argv[2] = {name, pd};
        def->Call(obj, 2, argv);

        if (firstTry.HasCaught()) {
          return firstTry.ReThrow();
        }
      }
    }
    return True();
  }
  return False();
}

/**
 *  Function used for a constructor and invocation
 *  handler of a Proxy created function
 *  Calls the appropriate function attached when the Proxy was created
 *
 *  @param ...args
 *  @returns mixed
 *  @throws Error
 */
Handle<Value> NodeProxy::New(const Arguments& args) {
  HandleScope scope;

  if (args.Callee()->InternalFieldCount() < 1 && args.Data().IsEmpty()) {
    return THR_TYPE_ERROR("defineProperty expects first "
                "argument to be intialized by Proxy");
  }

  Local<Value> info, ret, data =  args.Holder()->GetInternalField(0);

  if (data.IsEmpty() || !data->IsObject()) {
    return THREXC("Invalid reference to Proxy#constructor");
  }

  Local<Function> fn;
  Local<Object> obj = data->ToObject();

  if (args.IsConstructCall()) {
    info = obj->GetHiddenValue(NodeProxy::constructorTrap);

    if (!info.IsEmpty() && info->IsFunction()) {
      fn = Local<Function>::Cast(info);
    } else {
      fn = Local<Function>::Cast(
          obj->GetHiddenValue(NodeProxy::callTrap));
    }
  } else {
    fn = Local<Function>::Cast(obj->GetHiddenValue(NodeProxy::callTrap));
  }

  int i = 0, l = args.Length();
  Local<Value>* argv = new Local<Value>[l];

  for (; i < l; ++i) {
    argv[i] = args[i];
  }

  ret = fn->Call(args.This(), args.Length(), argv);

  if (args.IsConstructCall()) {
    if (!ret.IsEmpty()) {
      return ret;
    }
    return args.This();
  }
  return ret;
}

/**
 *  Invoked for accessing the named properties of an object
 *
 *
 *
 */
Handle<Value> NodeProxy::GetNamedProperty(Local<String> name,
                      const AccessorInfo &info) {
  HandleScope scope;

  if (info.This()->InternalFieldCount() < 1 || info.Data().IsEmpty()) {
    return THR_TYPE_ERROR("SetNamedProperty intercepted "
                "by non-Proxy object");
  }

  Local<Value> argv1[1] = {name};
  Local<Value> data = info.This()->InternalFieldCount() > 0 ?
                      info.This()->GetInternalField(0) :
                      info.Data();

  if (!data->IsObject()) {
    return Undefined();
  }

  Local<Function> fn;
  Local<Object> handler = data->ToObject();

  // if the Proxy isn't trapping, return
  // the value set on the property descriptor
  if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
    return CallPropertyDescriptorGet(handler->Get(name), info.This(), argv1);
  }

  Local<Value> get = handler->Get(NodeProxy::get);
  if (get->IsFunction()) {
    fn = Local<Function>::Cast(get);
    Local<Value> argv[2] = {info.This(), name};

    return fn->Call(handler, 2, argv);
  }

  Local<Value> getPropertyDescriptor = handler->Get(NodeProxy::getPropertyDescriptor);
  if (getPropertyDescriptor->IsFunction()) {
    fn = Local<Function>::Cast(getPropertyDescriptor);

    return CallPropertyDescriptorGet(fn->Call(handler, 1, argv1), info.This(), argv1);
  }

  Local<Value> getOwnPropertyDescriptor = handler->Get(NodeProxy::getOwnPropertyDescriptor);
  if (getOwnPropertyDescriptor->IsFunction()) {
    fn = Local<Function>::Cast(getOwnPropertyDescriptor);

    return CallPropertyDescriptorGet(fn->Call(handler, 1, argv1), info.This(), argv1);
  }
}

Local<Value> NodeProxy::CallPropertyDescriptorGet(Local<Value> descriptor, Handle<Object> context, Local<Value> args[1]) {
  if (descriptor->IsObject()) {
    Local<Value> get = descriptor->ToObject()->Get(NodeProxy::get);

    if (get->IsFunction()) {
      Local<Function> fn = Local<Function>::Cast(get);
      return fn->Call(context, 1, args);
    }

    return descriptor->ToObject()->Get(NodeProxy::value);
  }

  return Local<Value>::New(Undefined());
}

/**
 *  Invoked for setting the named properties of an object
 *
 *
 *
 */
Handle<Value> NodeProxy::SetNamedProperty(Local<String> name,
                      Local<Value> value,
                      const AccessorInfo &info) {
  HandleScope scope;

  if (info.This()->InternalFieldCount() < 1 || info.Data().IsEmpty()) {
    return THR_TYPE_ERROR("SetNamedProperty intercepted "
                "by non-Proxy object");
  }

  Local<Value> argv2[2] = {name, value};
  Local<Value> data = info.This()->InternalFieldCount() > 0 ?
                      info.This()->GetInternalField(0) :
                      info.Data();

  if (!data->IsObject()) {
    return Undefined();
  }

  Local<Object> handler = data->ToObject();

  // if the Proxy isn't trapping, return the
  // value set on the property descriptor
  if (!handler->GetHiddenValue(NodeProxy::trapping)->BooleanValue()) {
    if (handler->GetHiddenValue(NodeProxy::extensible)->BooleanValue() ||
      handler->Has(name)
    ) {
      Local<Value> pd = handler->Get(name);

      if (!pd->IsObject()) {
        return Undefined();
      }

      Local<Object> pd_obj = pd->ToObject();

      if (!pd_obj->GetHiddenValue(
            NodeProxy::writable)->BooleanValue()
      ) {
        return THREXCW(
              String::Concat(
                String::New("In accessible property: "),
                    name));
      }

      Local<Value> set = pd_obj->Get(NodeProxy::set);
      if (set->IsFunction()) {
        Local<Function> fn = Local<Function>::Cast(set);
        fn->Call(info.This(), 2, argv2);

        return value;
      }

      if (pd_obj->Set(NodeProxy::value, value)) {
        return value;
      }
      return Undefined();
    }
    return Undefined();
  }

  