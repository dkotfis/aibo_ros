/*! \file urbi/ucallbacks.hh
 *******************************************************************************

 Definition of the callback classes.

 This file is part of UObject Component Architecture\n
 Copyright (c) 2006, 2007, 2008 Gostai S.A.S.

 Permission to use, copy, modify, and redistribute this software for
 non-commercial use is hereby granted.

 This software is provided "as is" without warranty of any kind,
 either expressed or implied, including but not limited to the
 implied warranties of fitness for a particular purpose.

 For more information, comments, bug reports: http://www.urbiforge.com

 **************************************************************************** */

#ifndef URBI_UCALLBACKS_HH
# define URBI_UCALLBACKS_HH

# include <list>
# include <string>

# include "libport/singleton-ptr.hh"
# include "libport/hash.hh"

# include "urbi/uvalue.hh"

namespace urbi
{

  // A few list and hashtable types
  typedef libport::hash_map_type<std::string,
				 std::list<UGenericCallback*> >::type UTable;

  //! Clean a callback UTable from all callbacks linked to the
  //! object whose name is 'name'.
  void cleanTable(UTable &t, const std::string& name);

  typedef libport::hash_map_type<std::string, std::list<UVar*> >::type UVarTable;
  typedef std::list<UTimerCallback*> UTimerTable;

  // Lists and hashtables used.
  EXTERN_STATIC_INSTANCE(UTable, accessmap);
  EXTERN_STATIC_INSTANCE(UTable, eventendmap);
  EXTERN_STATIC_INSTANCE(UTable, eventmap);
  EXTERN_STATIC_INSTANCE(UTable, functionmap);
  EXTERN_STATIC_INSTANCE(UTable, monitormap);
  EXTERN_STATIC_INSTANCE(UVarTable, varmap);

  // Timer and update maps.
  EXTERN_STATIC_INSTANCE(UTimerTable, timermap);
  EXTERN_STATIC_INSTANCE(UTimerTable, updatemap);

  //! Function and Event storage mechanism
  /*! This heavily overloaded class is the only way in C++ to make life easy from the
    the interface user point's of view.
  */

  class UGenericCallback
  {
  public:
    UGenericCallback(const std::string& objname,
		     const std::string& type,
		     const std::string& name,
		     int size, UTable&, bool owned);
    UGenericCallback(const std::string& objname,
		     const std::string& type,
		     const std::string& name, UTable&);
    virtual ~UGenericCallback();

    void registerCallback(UTable& t);
    std::string getName() { return name;};

    virtual UValue __evalcall(UList &param)  = 0;

    /// Used to store the UVar* pointeur for var monitoring.
    void* storage;
    /// Period of timers.
    ufloat period;
    /// Nb params of the callbacked function.
    int    nbparam;
    /// Name of the UObject that has created the callback.
    std::string objname;

  private:
    std::string name;
  };


  //! Timer mechanism
  /*! This class stores a callback as a class method
   */

  class UTimerCallback
  {
  public:
    UTimerCallback(const std::string& objname, ufloat period, UTimerTable& tt);
    virtual ~UTimerCallback();

    virtual void call() = 0;

    ufloat period;
    ufloat lastTimeCalled;
    std::string objname;
  };

  // UTimerCallback subclasses

  template <class T>
  class UTimerCallbackobj : public UTimerCallback
  {
  public:
# define MKUTimerCallBackObj(Const, IsConst)				\
    UTimerCallbackobj(const std::string& objname,			\
		      ufloat period, T* obj,				\
		      int (T::*fun) () Const, UTimerTable &tt)		\
      : UTimerCallback(objname, period,tt), obj(obj),			\
	fun##Const(fun), is_const_ (IsConst)				\
    {}

    MKUTimerCallBackObj (/**/, false);
    MKUTimerCallBackObj (const, true);

# undef MKUTimerCallBackObj

    virtual void call()
    {
      (is_const_) ? ((*obj).*funconst)() : ((*obj).*fun)();
    }
  private:
    T* obj;
    int (T::*fun) ();
    int (T::*funconst) () const;
    bool is_const_;
  };


  /*------------------------------------------------.
  | This section is generated. Not for human eyes.  |
  `------------------------------------------------*/


  // non void return type

  template <class OBJ, class R>
  class UCallback0 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr0(Const, IsConst)			\
      UCallback0(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) () Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 0, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr0 (/**/, false);
    MKUCallBackNotifyCtr0 (const, true);

# undef MKUCallBackNotifyCtr0

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)()
	: ((*obj).*fun)     ();
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ();
    R (OBJ::*funconst) () const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ>
  class UCallbackvoid0 : public UGenericCallback
  {
    public:
    UCallbackvoid0(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) (),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,0, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)();
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ();
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ>
  class UCallbacknotifyend0 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr0(FConst, EConst, IsConst)		\
      UCallbacknotifyend0(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) () FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,0, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr0 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr0 (/**/, const, true);
    MKUCallBackNotifyEndCtr0 (const ,/**/, false);
    MKUCallBackNotifyEndCtr0 (const, const, true);

# undef MKUCallBackNotifyEndCtr0

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU >
  class UFCallbackvoid0 : public UGenericCallback
  {
    public:
    UFCallbackvoid0(const std::string& objname, const std::string& type,
		      void (*fun) (),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,0, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)();
      return UValue();
    }

    private:
    void (*fun) ();
  };

#endif

  // non void non-member function
  template <class R>
  class UFCallback0 : public UGenericCallback
  {
    public:
    UFCallback0(const std::string& objname, const std::string& type,
		  R (*fun) (),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,0, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)();
      return res;
    }

    private:
    R (*fun) ();
  };

  // callback creation for non-member non void return type

  template <class R>
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) (),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback0<R> (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template <> inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) (),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid0<void > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack0(Const)					\
  template <class OBJ, class R>				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) () Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback0<OBJ,R> (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack0 (/**/);
  MKUCreateCallBack0 (const);

# undef MKUCreateCallBack0

#if 0
  // callback creation for void return type

  template <class OBJ>
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) (),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid0<OBJ> (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd0(FConst, EConst)			\
  template <class OBJ>					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) () FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend0<OBJ> (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd0 (/**/, /**/);
  MKCreateUCallBackEnd0 (/**/, const);
  MKCreateUCallBackEnd0 (const, /**/);
  MKCreateUCallBackEnd0 (const, const);

# undef MKCreateUCallBackEnd0


  // non void return type

  template <class OBJ, class R, class P1 >
  class UCallback1 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr1(Const, IsConst)			\
      UCallback1(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 1, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr1 (/**/, false);
    MKUCallBackNotifyCtr1 (const, true);

# undef MKUCallBackNotifyCtr1

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 );
    R (OBJ::*funconst) ( P1 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 >
  class UCallbackvoid1 : public UGenericCallback
  {
    public:
    UCallbackvoid1(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,1, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 >
  class UCallbacknotifyend1 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr1(FConst, EConst, IsConst)		\
      UCallbacknotifyend1(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,1, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr1 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr1 (/**/, const, true);
    MKUCallBackNotifyEndCtr1 (const ,/**/, false);
    MKUCallBackNotifyEndCtr1 (const, const, true);

# undef MKUCallBackNotifyEndCtr1

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 >
  class UFCallbackvoid1 : public UGenericCallback
  {
    public:
    UFCallbackvoid1(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,1, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 );
  };

#endif

  // non void non-member function
  template <class R, class P1 >
  class UFCallback1 : public UGenericCallback
  {
    public:
    UFCallback1(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,1, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback1<R, P1 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid1<void , P1 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack1(Const)					\
  template <class OBJ, class R, class P1 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback1<OBJ,R, P1 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack1 (/**/);
  MKUCreateCallBack1 (const);

# undef MKUCreateCallBack1

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid1<OBJ, P1 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd1(FConst, EConst)			\
  template <class OBJ, class P1 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend1<OBJ, P1 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd1 (/**/, /**/);
  MKCreateUCallBackEnd1 (/**/, const);
  MKCreateUCallBackEnd1 (const, /**/);
  MKCreateUCallBackEnd1 (const, const);

# undef MKCreateUCallBackEnd1


  // non void return type

  template <class OBJ, class R, class P1 , class P2 >
  class UCallback2 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr2(Const, IsConst)			\
      UCallback2(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 2, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr2 (/**/, false);
    MKUCallBackNotifyCtr2 (const, true);

# undef MKUCallBackNotifyCtr2

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 );
    R (OBJ::*funconst) ( P1 , P2 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 >
  class UCallbackvoid2 : public UGenericCallback
  {
    public:
    UCallbackvoid2(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,2, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 >
  class UCallbacknotifyend2 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr2(FConst, EConst, IsConst)		\
      UCallbacknotifyend2(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,2, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr2 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr2 (/**/, const, true);
    MKUCallBackNotifyEndCtr2 (const ,/**/, false);
    MKUCallBackNotifyEndCtr2 (const, const, true);

# undef MKUCallBackNotifyEndCtr2

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 >
  class UFCallbackvoid2 : public UGenericCallback
  {
    public:
    UFCallbackvoid2(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,2, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 >
  class UFCallback2 : public UGenericCallback
  {
    public:
    UFCallback2(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,2, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback2<R, P1 , P2 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid2<void , P1 , P2 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack2(Const)					\
  template <class OBJ, class R, class P1 , class P2 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback2<OBJ,R, P1 , P2 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack2 (/**/);
  MKUCreateCallBack2 (const);

# undef MKUCreateCallBack2

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid2<OBJ, P1 , P2 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd2(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend2<OBJ, P1 , P2 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd2 (/**/, /**/);
  MKCreateUCallBackEnd2 (/**/, const);
  MKCreateUCallBackEnd2 (const, /**/);
  MKCreateUCallBackEnd2 (const, const);

# undef MKCreateUCallBackEnd2


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 >
  class UCallback3 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr3(Const, IsConst)			\
      UCallback3(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 3, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr3 (/**/, false);
    MKUCallBackNotifyCtr3 (const, true);

# undef MKUCallBackNotifyCtr3

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 );
    R (OBJ::*funconst) ( P1 , P2 , P3 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 >
  class UCallbackvoid3 : public UGenericCallback
  {
    public:
    UCallbackvoid3(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,3, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 >
  class UCallbacknotifyend3 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr3(FConst, EConst, IsConst)		\
      UCallbacknotifyend3(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,3, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr3 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr3 (/**/, const, true);
    MKUCallBackNotifyEndCtr3 (const ,/**/, false);
    MKUCallBackNotifyEndCtr3 (const, const, true);

# undef MKUCallBackNotifyEndCtr3

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 >
  class UFCallbackvoid3 : public UGenericCallback
  {
    public:
    UFCallbackvoid3(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,3, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 >
  class UFCallback3 : public UGenericCallback
  {
    public:
    UFCallback3(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,3, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback3<R, P1 , P2 , P3 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid3<void , P1 , P2 , P3 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack3(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback3<OBJ,R, P1 , P2 , P3 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack3 (/**/);
  MKUCreateCallBack3 (const);

# undef MKUCreateCallBack3

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid3<OBJ, P1 , P2 , P3 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd3(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend3<OBJ, P1 , P2 , P3 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd3 (/**/, /**/);
  MKCreateUCallBackEnd3 (/**/, const);
  MKCreateUCallBackEnd3 (const, /**/);
  MKCreateUCallBackEnd3 (const, const);

# undef MKCreateUCallBackEnd3


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 >
  class UCallback4 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr4(Const, IsConst)			\
      UCallback4(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 4, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr4 (/**/, false);
    MKUCallBackNotifyCtr4 (const, true);

# undef MKUCallBackNotifyCtr4

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 >
  class UCallbackvoid4 : public UGenericCallback
  {
    public:
    UCallbackvoid4(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,4, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 >
  class UCallbacknotifyend4 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr4(FConst, EConst, IsConst)		\
      UCallbacknotifyend4(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,4, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr4 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr4 (/**/, const, true);
    MKUCallBackNotifyEndCtr4 (const ,/**/, false);
    MKUCallBackNotifyEndCtr4 (const, const, true);

# undef MKUCallBackNotifyEndCtr4

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 >
  class UFCallbackvoid4 : public UGenericCallback
  {
    public:
    UFCallbackvoid4(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,4, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 >
  class UFCallback4 : public UGenericCallback
  {
    public:
    UFCallback4(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,4, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback4<R, P1 , P2 , P3 , P4 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid4<void , P1 , P2 , P3 , P4 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack4(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback4<OBJ,R, P1 , P2 , P3 , P4 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack4 (/**/);
  MKUCreateCallBack4 (const);

# undef MKUCreateCallBack4

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid4<OBJ, P1 , P2 , P3 , P4 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd4(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend4<OBJ, P1 , P2 , P3 , P4 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd4 (/**/, /**/);
  MKCreateUCallBackEnd4 (/**/, const);
  MKCreateUCallBackEnd4 (const, /**/);
  MKCreateUCallBackEnd4 (const, const);

# undef MKCreateUCallBackEnd4


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 >
  class UCallback5 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr5(Const, IsConst)			\
      UCallback5(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 5, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr5 (/**/, false);
    MKUCallBackNotifyCtr5 (const, true);

# undef MKUCallBackNotifyCtr5

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 >
  class UCallbackvoid5 : public UGenericCallback
  {
    public:
    UCallbackvoid5(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,5, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 >
  class UCallbacknotifyend5 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr5(FConst, EConst, IsConst)		\
      UCallbacknotifyend5(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,5, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr5 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr5 (/**/, const, true);
    MKUCallBackNotifyEndCtr5 (const ,/**/, false);
    MKUCallBackNotifyEndCtr5 (const, const, true);

# undef MKUCallBackNotifyEndCtr5

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 >
  class UFCallbackvoid5 : public UGenericCallback
  {
    public:
    UFCallbackvoid5(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,5, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 >
  class UFCallback5 : public UGenericCallback
  {
    public:
    UFCallback5(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,5, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback5<R, P1 , P2 , P3 , P4 , P5 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid5<void , P1 , P2 , P3 , P4 , P5 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack5(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback5<OBJ,R, P1 , P2 , P3 , P4 , P5 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack5 (/**/);
  MKUCreateCallBack5 (const);

# undef MKUCreateCallBack5

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid5<OBJ, P1 , P2 , P3 , P4 , P5 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd5(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend5<OBJ, P1 , P2 , P3 , P4 , P5 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd5 (/**/, /**/);
  MKCreateUCallBackEnd5 (/**/, const);
  MKCreateUCallBackEnd5 (const, /**/);
  MKCreateUCallBackEnd5 (const, const);

# undef MKCreateUCallBackEnd5


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  class UCallback6 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr6(Const, IsConst)			\
      UCallback6(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 6, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr6 (/**/, false);
    MKUCallBackNotifyCtr6 (const, true);

# undef MKUCallBackNotifyCtr6

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  class UCallbackvoid6 : public UGenericCallback
  {
    public:
    UCallbackvoid6(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,6, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  class UCallbacknotifyend6 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr6(FConst, EConst, IsConst)		\
      UCallbacknotifyend6(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,6, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr6 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr6 (/**/, const, true);
    MKUCallBackNotifyEndCtr6 (const ,/**/, false);
    MKUCallBackNotifyEndCtr6 (const, const, true);

# undef MKUCallBackNotifyEndCtr6

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  class UFCallbackvoid6 : public UGenericCallback
  {
    public:
    UFCallbackvoid6(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,6, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  class UFCallback6 : public UGenericCallback
  {
    public:
    UFCallback6(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,6, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback6<R, P1 , P2 , P3 , P4 , P5 , P6 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid6<void , P1 , P2 , P3 , P4 , P5 , P6 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack6(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback6<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack6 (/**/);
  MKUCreateCallBack6 (const);

# undef MKUCreateCallBack6

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid6<OBJ, P1 , P2 , P3 , P4 , P5 , P6 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd6(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend6<OBJ, P1 , P2 , P3 , P4 , P5 , P6 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd6 (/**/, /**/);
  MKCreateUCallBackEnd6 (/**/, const);
  MKCreateUCallBackEnd6 (const, /**/);
  MKCreateUCallBackEnd6 (const, const);

# undef MKCreateUCallBackEnd6


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  class UCallback7 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr7(Const, IsConst)			\
      UCallback7(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 7, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr7 (/**/, false);
    MKUCallBackNotifyCtr7 (const, true);

# undef MKUCallBackNotifyCtr7

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  class UCallbackvoid7 : public UGenericCallback
  {
    public:
    UCallbackvoid7(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,7, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  class UCallbacknotifyend7 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr7(FConst, EConst, IsConst)		\
      UCallbacknotifyend7(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,7, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr7 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr7 (/**/, const, true);
    MKUCallBackNotifyEndCtr7 (const ,/**/, false);
    MKUCallBackNotifyEndCtr7 (const, const, true);

# undef MKUCallBackNotifyEndCtr7

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  class UFCallbackvoid7 : public UGenericCallback
  {
    public:
    UFCallbackvoid7(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,7, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  class UFCallback7 : public UGenericCallback
  {
    public:
    UFCallback7(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,7, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback7<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid7<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack7(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback7<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack7 (/**/);
  MKUCreateCallBack7 (const);

# undef MKUCreateCallBack7

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid7<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd7(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend7<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd7 (/**/, /**/);
  MKCreateUCallBackEnd7 (/**/, const);
  MKCreateUCallBackEnd7 (const, /**/);
  MKCreateUCallBackEnd7 (const, const);

# undef MKCreateUCallBackEnd7


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  class UCallback8 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr8(Const, IsConst)			\
      UCallback8(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 8, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr8 (/**/, false);
    MKUCallBackNotifyCtr8 (const, true);

# undef MKUCallBackNotifyCtr8

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  class UCallbackvoid8 : public UGenericCallback
  {
    public:
    UCallbackvoid8(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,8, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  class UCallbacknotifyend8 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr8(FConst, EConst, IsConst)		\
      UCallbacknotifyend8(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,8, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr8 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr8 (/**/, const, true);
    MKUCallBackNotifyEndCtr8 (const ,/**/, false);
    MKUCallBackNotifyEndCtr8 (const, const, true);

# undef MKUCallBackNotifyEndCtr8

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  class UFCallbackvoid8 : public UGenericCallback
  {
    public:
    UFCallbackvoid8(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,8, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  class UFCallback8 : public UGenericCallback
  {
    public:
    UFCallback8(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,8, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback8<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid8<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack8(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback8<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack8 (/**/);
  MKUCreateCallBack8 (const);

# undef MKUCreateCallBack8

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid8<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd8(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend8<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd8 (/**/, /**/);
  MKCreateUCallBackEnd8 (/**/, const);
  MKCreateUCallBackEnd8 (const, /**/);
  MKCreateUCallBackEnd8 (const, const);

# undef MKCreateUCallBackEnd8


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  class UCallback9 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr9(Const, IsConst)			\
      UCallback9(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 9, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr9 (/**/, false);
    MKUCallBackNotifyCtr9 (const, true);

# undef MKUCallBackNotifyCtr9

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  class UCallbackvoid9 : public UGenericCallback
  {
    public:
    UCallbackvoid9(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,9, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  class UCallbacknotifyend9 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr9(FConst, EConst, IsConst)		\
      UCallbacknotifyend9(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,9, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr9 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr9 (/**/, const, true);
    MKUCallBackNotifyEndCtr9 (const ,/**/, false);
    MKUCallBackNotifyEndCtr9 (const, const, true);

# undef MKUCallBackNotifyEndCtr9

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  class UFCallbackvoid9 : public UGenericCallback
  {
    public:
    UFCallbackvoid9(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,9, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  class UFCallback9 : public UGenericCallback
  {
    public:
    UFCallback9(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,9, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback9<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid9<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack9(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback9<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack9 (/**/);
  MKUCreateCallBack9 (const);

# undef MKUCreateCallBack9

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid9<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd9(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend9<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd9 (/**/, /**/);
  MKCreateUCallBackEnd9 (/**/, const);
  MKCreateUCallBackEnd9 (const, /**/);
  MKCreateUCallBackEnd9 (const, const);

# undef MKCreateUCallBackEnd9


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  class UCallback10 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr10(Const, IsConst)			\
      UCallback10(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 10, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr10 (/**/, false);
    MKUCallBackNotifyCtr10 (const, true);

# undef MKUCallBackNotifyCtr10

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  class UCallbackvoid10 : public UGenericCallback
  {
    public:
    UCallbackvoid10(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,10, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  class UCallbacknotifyend10 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr10(FConst, EConst, IsConst)		\
      UCallbacknotifyend10(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,10, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr10 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr10 (/**/, const, true);
    MKUCallBackNotifyEndCtr10 (const ,/**/, false);
    MKUCallBackNotifyEndCtr10 (const, const, true);

# undef MKUCallBackNotifyEndCtr10

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  class UFCallbackvoid10 : public UGenericCallback
  {
    public:
    UFCallbackvoid10(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,10, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  class UFCallback10 : public UGenericCallback
  {
    public:
    UFCallback10(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,10, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback10<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid10<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack10(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback10<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack10 (/**/);
  MKUCreateCallBack10 (const);

# undef MKUCreateCallBack10

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid10<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd10(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend10<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd10 (/**/, /**/);
  MKCreateUCallBackEnd10 (/**/, const);
  MKCreateUCallBackEnd10 (const, /**/);
  MKCreateUCallBackEnd10 (const, const);

# undef MKCreateUCallBackEnd10


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  class UCallback11 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr11(Const, IsConst)			\
      UCallback11(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 11, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr11 (/**/, false);
    MKUCallBackNotifyCtr11 (const, true);

# undef MKUCallBackNotifyCtr11

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  class UCallbackvoid11 : public UGenericCallback
  {
    public:
    UCallbackvoid11(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,11, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  class UCallbacknotifyend11 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr11(FConst, EConst, IsConst)		\
      UCallbacknotifyend11(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,11, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr11 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr11 (/**/, const, true);
    MKUCallBackNotifyEndCtr11 (const ,/**/, false);
    MKUCallBackNotifyEndCtr11 (const, const, true);

# undef MKUCallBackNotifyEndCtr11

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  class UFCallbackvoid11 : public UGenericCallback
  {
    public:
    UFCallbackvoid11(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,11, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  class UFCallback11 : public UGenericCallback
  {
    public:
    UFCallback11(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,11, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback11<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid11<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack11(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback11<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack11 (/**/);
  MKUCreateCallBack11 (const);

# undef MKUCreateCallBack11

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid11<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd11(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend11<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd11 (/**/, /**/);
  MKCreateUCallBackEnd11 (/**/, const);
  MKCreateUCallBackEnd11 (const, /**/);
  MKCreateUCallBackEnd11 (const, const);

# undef MKCreateUCallBackEnd11


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  class UCallback12 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr12(Const, IsConst)			\
      UCallback12(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 12, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr12 (/**/, false);
    MKUCallBackNotifyCtr12 (const, true);

# undef MKUCallBackNotifyCtr12

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  class UCallbackvoid12 : public UGenericCallback
  {
    public:
    UCallbackvoid12(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,12, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  class UCallbacknotifyend12 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr12(FConst, EConst, IsConst)		\
      UCallbacknotifyend12(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,12, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr12 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr12 (/**/, const, true);
    MKUCallBackNotifyEndCtr12 (const ,/**/, false);
    MKUCallBackNotifyEndCtr12 (const, const, true);

# undef MKUCallBackNotifyEndCtr12

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  class UFCallbackvoid12 : public UGenericCallback
  {
    public:
    UFCallbackvoid12(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,12, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  class UFCallback12 : public UGenericCallback
  {
    public:
    UFCallback12(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,12, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback12<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid12<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack12(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback12<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack12 (/**/);
  MKUCreateCallBack12 (const);

# undef MKUCreateCallBack12

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid12<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd12(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend12<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd12 (/**/, /**/);
  MKCreateUCallBackEnd12 (/**/, const);
  MKCreateUCallBackEnd12 (const, /**/);
  MKCreateUCallBackEnd12 (const, const);

# undef MKCreateUCallBackEnd12


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  class UCallback13 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr13(Const, IsConst)			\
      UCallback13(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 13, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr13 (/**/, false);
    MKUCallBackNotifyCtr13 (const, true);

# undef MKUCallBackNotifyCtr13

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  class UCallbackvoid13 : public UGenericCallback
  {
    public:
    UCallbackvoid13(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,13, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  class UCallbacknotifyend13 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr13(FConst, EConst, IsConst)		\
      UCallbacknotifyend13(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,13, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr13 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr13 (/**/, const, true);
    MKUCallBackNotifyEndCtr13 (const ,/**/, false);
    MKUCallBackNotifyEndCtr13 (const, const, true);

# undef MKUCallBackNotifyEndCtr13

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  class UFCallbackvoid13 : public UGenericCallback
  {
    public:
    UFCallbackvoid13(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,13, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  class UFCallback13 : public UGenericCallback
  {
    public:
    UFCallback13(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,13, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback13<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid13<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack13(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback13<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack13 (/**/);
  MKUCreateCallBack13 (const);

# undef MKUCreateCallBack13

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid13<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd13(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend13<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd13 (/**/, /**/);
  MKCreateUCallBackEnd13 (/**/, const);
  MKCreateUCallBackEnd13 (const, /**/);
  MKCreateUCallBackEnd13 (const, const);

# undef MKCreateUCallBackEnd13


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  class UCallback14 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr14(Const, IsConst)			\
      UCallback14(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 14, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr14 (/**/, false);
    MKUCallBackNotifyCtr14 (const, true);

# undef MKUCallBackNotifyCtr14

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  class UCallbackvoid14 : public UGenericCallback
  {
    public:
    UCallbackvoid14(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,14, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  class UCallbacknotifyend14 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr14(FConst, EConst, IsConst)		\
      UCallbacknotifyend14(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,14, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr14 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr14 (/**/, const, true);
    MKUCallBackNotifyEndCtr14 (const ,/**/, false);
    MKUCallBackNotifyEndCtr14 (const, const, true);

# undef MKUCallBackNotifyEndCtr14

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  class UFCallbackvoid14 : public UGenericCallback
  {
    public:
    UFCallbackvoid14(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,14, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  class UFCallback14 : public UGenericCallback
  {
    public:
    UFCallback14(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,14, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback14<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid14<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack14(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback14<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack14 (/**/);
  MKUCreateCallBack14 (const);

# undef MKUCreateCallBack14

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid14<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd14(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend14<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd14 (/**/, /**/);
  MKCreateUCallBackEnd14 (/**/, const);
  MKCreateUCallBackEnd14 (const, /**/);
  MKCreateUCallBackEnd14 (const, const);

# undef MKCreateUCallBackEnd14


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  class UCallback15 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr15(Const, IsConst)			\
      UCallback15(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 15, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr15 (/**/, false);
    MKUCallBackNotifyCtr15 (const, true);

# undef MKUCallBackNotifyCtr15

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  class UCallbackvoid15 : public UGenericCallback
  {
    public:
    UCallbackvoid15(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,15, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  class UCallbacknotifyend15 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr15(FConst, EConst, IsConst)		\
      UCallbacknotifyend15(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,15, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr15 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr15 (/**/, const, true);
    MKUCallBackNotifyEndCtr15 (const ,/**/, false);
    MKUCallBackNotifyEndCtr15 (const, const, true);

# undef MKUCallBackNotifyEndCtr15

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  class UFCallbackvoid15 : public UGenericCallback
  {
    public:
    UFCallbackvoid15(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,15, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  class UFCallback15 : public UGenericCallback
  {
    public:
    UFCallback15(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,15, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback15<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid15<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack15(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback15<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack15 (/**/);
  MKUCreateCallBack15 (const);

# undef MKUCreateCallBack15

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid15<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd15(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend15<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd15 (/**/, /**/);
  MKCreateUCallBackEnd15 (/**/, const);
  MKCreateUCallBackEnd15 (const, /**/);
  MKCreateUCallBackEnd15 (const, const);

# undef MKCreateUCallBackEnd15


  // non void return type

  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  class UCallback16 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyCtr16(Const, IsConst)			\
      UCallback16(const std::string& objname, const std::string& type,	\
		   OBJ* obj,						\
		   R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ) Const,			\
		   const std::string& funname, UTable &t, bool owned)	\
      : UGenericCallback(objname, type, funname, 16, t, owned),	\
	obj(obj), fun##Const(fun), is_const_ (IsConst)			\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyCtr16 (/**/, false);
    MKUCallBackNotifyCtr16 (const, true);

# undef MKUCallBackNotifyCtr16

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(),
	(is_const_)
	? ((*obj).*funconst)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) , uvalue_cast<P16>(param[16 - 1]) )
	: ((*obj).*fun)     ( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) , uvalue_cast<P16>(param[16 - 1]) );
      return res;
    }

    private:
    OBJ* obj;
    R (OBJ::*fun)      ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 );
    R (OBJ::*funconst) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ) const;
    bool is_const_;
  };

#if 0
  // void return type
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  class UCallbackvoid16 : public UGenericCallback
  {
    public:
    UCallbackvoid16(const std::string& objname, const std::string& type,
		     OBJ* obj,
		     void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		   const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,16, t, owned),
	obj(obj), fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      ((*obj).*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) , uvalue_cast<P16>(param[16 - 1]) );
      return UValue();
    }
    private:
    OBJ* obj;
    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 );
  };
#endif


  // void return type : special case for notifyend event callbacks
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  class UCallbacknotifyend16 : public UGenericCallback
  {
    public:

# define MKUCallBackNotifyEndCtr16(FConst, EConst, IsConst)		\
      UCallbacknotifyend16(const std::string& objname,			\
			    const std::string& type,			\
			    OBJ* obj,					\
			    void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ) FConst,	\
			    void (OBJ::*end)() EConst,			\
			    const std::string& funname,			\
			    UTable &t)					\
      : UGenericCallback(objname, type, funname,16, t, false),		\
	obj(obj), fun(end), is_const_(IsConst)				\
    {									\
      registerCallback(t);						\
    }

    MKUCallBackNotifyEndCtr16 (/**/, /**/, false);
    MKUCallBackNotifyEndCtr16 (/**/, const, true);
    MKUCallBackNotifyEndCtr16 (const ,/**/, false);
    MKUCallBackNotifyEndCtr16 (const, const, true);

# undef MKUCallBackNotifyEndCtr16

    virtual UValue __evalcall(UList &)
    {
      UValue res;
      res(), (is_const_) ? ((*obj).*func)() : ((*obj).*fun)();
      return res;
    }

    private:
    OBJ* obj;
    void (OBJ::*fun) ();
    void (OBJ::*func) ();
    bool is_const_;
  };

#if 0
  // void non-member function
  template <class INU , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  class UFCallbackvoid16 : public UGenericCallback
  {
    public:
    UFCallbackvoid16(const std::string& objname, const std::string& type,
		      void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		    const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,16, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList &param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) , uvalue_cast<P16>(param[16 - 1]) );
      return UValue();
    }

    private:
    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 );
  };

#endif

  // non void non-member function
  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  class UFCallback16 : public UGenericCallback
  {
    public:
    UFCallback16(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		const std::string& funname, UTable &t, bool owned)
      : UGenericCallback(objname, type, funname,16, t, owned),  fun(fun)
    {
      registerCallback(t);
    }

    virtual UValue __evalcall(UList& param)
    {
      // Avoid warnings when we don't use param.
      (void) param;
      UValue res;
      res(), (*fun)( uvalue_cast<P1>(param[1 - 1]) , uvalue_cast<P2>(param[2 - 1]) , uvalue_cast<P3>(param[3 - 1]) , uvalue_cast<P4>(param[4 - 1]) , uvalue_cast<P5>(param[5 - 1]) , uvalue_cast<P6>(param[6 - 1]) , uvalue_cast<P7>(param[7 - 1]) , uvalue_cast<P8>(param[8 - 1]) , uvalue_cast<P9>(param[9 - 1]) , uvalue_cast<P10>(param[10 - 1]) , uvalue_cast<P11>(param[11 - 1]) , uvalue_cast<P12>(param[12 - 1]) , uvalue_cast<P13>(param[13 - 1]) , uvalue_cast<P14>(param[14 - 1]) , uvalue_cast<P15>(param[15 - 1]) , uvalue_cast<P16>(param[16 - 1]) );
      return res;
    }

    private:
    R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 );
  };

  // callback creation for non-member non void return type

  template <class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  R (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		  const std::string& funname, UTable& t, bool owned)
  {
    return new UFCallback16<R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 > (objname, type,fun,funname,t, owned);
  }

#if 0
  // callback creation for non-member void return type
  // inline required since one of them is not a template
  template < class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 > inline
    UGenericCallback*
    createUCallback(const std::string& objname, const std::string& type,
		    void (*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		    const std::string& funname, UTable &t, bool owned)
  {
    return new UFCallbackvoid16<void , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 > (objname, type,fun,funname,t,
						  owned);
  }
#endif

  // callback creation for non void return type

# define MKUCreateCallBack16(Const)					\
  template <class OBJ, class R, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >				\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  R (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ) Const,			\
		  const std::string& funname, UTable &t, bool owned)	\
  {									\
    return new UCallback16<OBJ,R, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 > (objname, type, obj, fun,	\
					     funname,t,			\
					     owned);			\
  }

  MKUCreateCallBack16 (/**/);
  MKUCreateCallBack16 (const);

# undef MKUCreateCallBack16

#if 0
  // callback creation for void return type

  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >
  UGenericCallback*
  createUCallback(const std::string& objname, const std::string& type,
		  OBJ* obj,
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ),
		  const std::string& funname, UTable &t, bool owned)
  {
    return new UCallbackvoid16<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 > (objname, type,obj,fun,funname,t,
					       owned);
  }
#endif

  // Special case for eventend notification
# define MKCreateUCallBackEnd16(FConst, EConst)			\
  template <class OBJ, class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 , class P10 , class P11 , class P12 , class P13 , class P14 , class P15 , class P16 >					\
  UGenericCallback*							\
  createUCallback(const std::string& objname, const std::string& type,	\
		  OBJ* obj,						\
		  void (OBJ::*fun) ( P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 ) FConst,		\
		  void (OBJ::*end)() EConst,				\
		  const std::string& funname, UTable &t)		\
  {									\
    return new UCallbacknotifyend16<OBJ, P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 , P10 , P11 , P12 , P13 , P14 , P15 , P16 > (objname,		\
						    type, obj, fun,	\
						    end,funname,t);	\
  }

  MKCreateUCallBackEnd16 (/**/, /**/);
  MKCreateUCallBackEnd16 (/**/, const);
  MKCreateUCallBackEnd16 (const, /**/);
  MKCreateUCallBackEnd16 (const, const);

# undef MKCreateUCallBackEnd16


} // end namespace urbi

#endif // ! URBI_UCALLBACKS_HH

/// Local Variables:
/// mode: c++
/// End:
