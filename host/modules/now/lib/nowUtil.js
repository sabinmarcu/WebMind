var util = {

  hasProperty: function (obj, prop) {
    return Object.prototype.hasOwnProperty.call(Object(obj), prop);
  },

  noop: function () {
  },

  clone: function (proto, obj) {
    // note regarding speed and C++ bindings: evidently it's slightly
    // faster to provide a wrapper function than adding an additional
    // level of lookup.
    if (!(obj && typeof obj === 'object')) {
      obj = {};
    }
    obj.__proto__ = proto;
    return obj;
  },

  generateRandomString: function () {
    return ("" + Math.random()).substring(2);
  },

  error: function (err) {
    console.log(err);
    if (util.hasProperty(err, 'stack')) {
      console.log(err.stack);
    }
  },

  getAllChildFqns: function (parentObj, parentFqn) {
    var fqns = [];
    var prop;
    function getAllChildFqnsHelper(parentObj, parentFqn) {
      for (var i = 0, keys = Object.keys(parentObj), ll = keys.length; i < ll; i++ ) {
        prop = keys[i];
        if (parentObj[prop] && typeof parentObj[prop] === 'object') {
          getAllChildFqnsHelper(parentObj[prop], parentFqn + '.' + prop);
        } else {
          fqns.push(parentFqn + '.' + prop);
        }
      }
      if (ll == 0) {
        fqns.push(parentFqn);
      }
    }
    getAllChildFqnsHelper(parentObj, parentFqn);
    return fqns;
  },

  extend: function (target, obj) {
    for (var i = 0, keys = Object.keys(obj), ii = keys.length; i < ii; i++) {
      var key = keys[i];
      if (obj[key] && typeof obj[key] === 'object') {
        if(Array.isArray(obj[key])) {
          target[key] = target[key] || [];
        } else {
          target[key] = target[key] || {};
        }
        util.extend(target[key], obj[key]);
      } else {
        target[key] = obj[key];
      }
    }
  },

  getVarFromFqn: function (fqn, scope) {
    var path = fqn.split('.');
    path.shift();
    var currVar = scope;
    var prop;
    while (path.length > 0) {
      prop = path.shift();
      if (util.hasProperty(currVar, prop)) {
        currVar = currVar[prop];
      } else {
        return false;
      }
    }
    return currVar;
  },

  stringify: JSON.stringify,
  parse: JSON.parse,

  flatten: function (val, fqn) {
    var vals = {};
    var fqns = util.getAllChildFqns(val, '');
    var m;
    for (var i = 0, ii = fqns.length; i < ii; i++) {
      m = util.getVarFromFqn(fqns[i], val);
      vals[fqn + fqns[i]] = m;
    }
    return vals;
  },

  getValOrFqn: function (val, fqn) {
    if (typeof val === 'function') {
      return {fqn: fqn};
    } else {
      return val;
    }
  },

  isEmptyObj: function (obj) {
    for (var i in obj) {
      return false;
    }
    return true;
  }
};

exports.nowUtil = util;
