/*! \file uabstractclient.hh
 ****************************************************************************
 * $Id: uabstractclient.hh,v 1.9 2005/09/30 17:48:00 nottale Exp $
 *
 * Definition of the URBI interface class
 *
 * Copyright (C) 2004, 2006, 2007, 2008 Jean-Christophe Baillie.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **********************************************************************/

#ifndef URBI_UABSTRACTCLIENT_HH
# define URBI_UABSTRACTCLIENT_HH

# include <cstdio>
# include <sys/types.h>
# include "libport/cstring"
# include <cstdlib>
# include <cstdarg>

# include <list>
# include <vector>
# include <iostream>
# include <string>

# include "libport/fwd.hh"

# include "urbi/fwd.hh"
# include "urbi/uobject.hh"
# include "urbi/uconversion.hh"

namespace urbi
{
  /// Return values for the callack functions.
  /*! Each callback function, when called, must return with either URBI_CONTINUE
    or URBI_REMOVE:
    - URBI_CONTINUE means that the client should continue to call this callbak
    function.
    - URBI_REMOVE means that the client should never call this callback again.
  */
  enum UCallbackAction
    {
      URBI_CONTINUE=0,
      URBI_REMOVE
    };

  /// Maximum length of an URBI tag.
  enum { URBI_MAX_TAG_LENGTH = 64 };

  typedef unsigned int UCallbackID;

# define DEBUG 0

# define UINVALIDCALLBACKID 0

  enum UMessageType
    {
      MESSAGE_SYSTEM, ///< Messages prefixed by ***.
      MESSAGE_ERROR,  ///< Messages prefixed by !!!.
      MESSAGE_DATA    ///< All other messages.
    };

  /// Class containing all informations related to an URBI message.
  class UMessage
  {
  public:
    /// Connection from which originated the message.
    UAbstractClient	 &client;
    /// Server-side timestamp.
    int		 timestamp;
    /// Associated tag.
    std::string	 tag;

    UMessageType	 type;

    /// Set only if the message type is MESSAGE_DATA.
    urbi::UValue	 *value;
    /// Set only if the message type is MESSAGE_SYSTEM or MESSAGE_ERROR.
    std::string	 message;
    /// Raw message without the binary data.
    std::string	 rawMessage;

    /// Default ctor
    UMessage(UAbstractClient & client);

    /// Parser constructor
    UMessage(UAbstractClient & client, int timestamp,
	     const char *tag, const char *message,
	     std::list<urbi::BinaryData> bins);
    /// If alocate is true, everything is copied, eles pointers are stolen
    UMessage(const UMessage &source);

    /// Free everything if data was copied, doesn't free anything otherwise
    ~UMessage();

    operator urbi::UValue& () {return *value;}

  };

  std::ostream& operator<<(std::ostream& s, const UMessage& m);

  /// Callback prototypes.
  typedef UCallbackAction (*UCallback)		   (const UMessage &msg);

  typedef UCallbackAction (*UCustomCallback)	   (void * callbackData,
						    const UMessage &msg);


  //used internaly
  class UCallbackInfo
  {
  public:
    char tag[URBI_MAX_TAG_LENGTH];
    UCallbackWrapper & callback;
    int id;
    bool operator ==(int id) const
    {
      return id==this->id;
    }
    UCallbackInfo(UCallbackWrapper &w)
      : callback(w)
    {}
  };
  //used internaly
  class UClientStreambuf;


  /// Interface for an URBI wrapper object.
  /*! Implementations of this interface are wrappers around the URBI protocol.
    It handles URBI messages parsing, callback registration and various
    formatting functions.
    Implementations of this interface should:
    - Redefine errorNotify() as a function able to notify the user of eventual
    errors.
    - Redfine the four mutual exclusion functions.
    - Redefine effectiveSend().
    - Fill recvBuffer, update recvBufferPosition and call processRecvBuffer()
    when new data is available.
    - Provide an execute() function in the namespace urbi, that never returns,
    and that will be called after initialization.

   See the liburbi-cpp documentation for more informations on
   how to use this class.
  */
  class UAbstractClient : public std::ostream
  {
  public:
    /// Connection Buffer size.
    enum { URBI_BUFLEN = 128000 };
    /// Standard port of URBI server.
    enum { URBI_PORT = 54000 } ;

    /// Create a new instance and connect to the Urbi server.
    UAbstractClient(const char *_host,
		    int _port = URBI_PORT,
		    int _buflen = URBI_BUFLEN);

    virtual ~UAbstractClient();

    /// Return current error status, or zero if no error occurred.
    int error()
    {
      return rc;
    }

    // Sending

    /// Function for backward compatibility. Will be removed in future versions.
    int send() { endPack(); return 0;}

    /// Send an Urbi command. The syntax is similar to the printf() function.
    int send(const char* format,...);

    ///send the value without any prefix or terminator
    int send(urbi::UValue& v);

    /// Send binary data.
    int sendBin(const void*, int len);

    /// Send an Urbi header followed by binary data.
    int sendBin(const void*, int len,const char * header,...);

    /// Lock the send buffer (for backward compatibility, will be removed in future versions).
    int startPack();

    /// Unlock the send buffer (for backward compatibility, will be removed in future versions).
    int endPack();

    /// Append Urbi commands to the send buffer (for backward compatibility, will be removed in future versions).
    int pack(const char*,...);

    /// va_list version of pack.
    int vpack(const char*,va_list args);

    /// Send urbi commands contained in a file.
    int sendFile(const char * filename);

    /// Send a command, prefixing it with a tag, and associate the given callback with this tag. */
    UCallbackID sendCommand(UCallback ,const char*,...);

    /// Send a command, prefixing it with a tag, and associate the given callback with this tag. */
    UCallbackID sendCommand(UCustomCallback ,void *,const char*,...);

    /// Send sound data to the robot for immediate playback.
    int sendSound(const char * device,
		  const urbi::USound &sound, const char * tag=0);

    /// Put a file on the robot's mass storage device.
    int putFile(const char * localName, const char * remoteName=0);

    /// Save a buffer to a file on the robot.
    int putFile(const void * buffer, int length, const char * remoteName);


    // Receiving

    /// Associate a callback function with a tag. new style
    UCallbackID setCallback(UCallbackWrapper & callback, const char * tag);

    /// Associate a callback function with all error messages from the server
    UCallbackID setErrorCallback(UCallbackWrapper & callback);

    /// Associate a callback with all messages
    UCallbackID setWildcardCallback(UCallbackWrapper & callback);

    /// Associate a callback with local connection errors
    UCallbackID setClientErrorCallback(UCallbackWrapper & callback);

    /// OLD-style callbacks
    UCallbackID setCallback(UCallback ,const char* tag);

    /// Associate a callback function with a tag, specifiing a callback custom value that will be passed back to the callback function.
    UCallbackID setCallback(UCustomCallback ,void * callbackData,const char* tag);

    /// Callback to class member functions(old-style).
    template<class C>						UCallbackID setCallback(C& ref,
											UCallbackAction (C::*)(			const UMessage &),		   const char * tag);
    template<class C, class P1>				UCallbackID setCallback(C& ref,
										UCallbackAction (C::*)(P1 ,		const UMessage &), P1,		   const char * tag);
    template<class C, class P1, class P2>			UCallbackID setCallback(C& ref,
											UCallbackAction (C::*)(P1 , P2,		const UMessage &), P1, P2,	   const char * tag);
    template<class C, class P1, class P2, class P3>		UCallbackID setCallback(C& ref,
											UCallbackAction (C::*)(P1 , P2, P3,	const UMessage &), P1, P2, P3,	   const char * tag);
    template<class C, class P1, class P2, class P3, class P4> UCallbackID setCallback(C& ref,
										      UCallbackAction (C::*)(P1 , P2, P3, P4, const UMessage &), P1, P2, P3, P4, const char * tag);

    /// Get the tag associated with a registered callback.
    int getAssociatedTag(UCallbackID id, char * tag);

    /// Delete a callback.
    int deleteCallback(UCallbackID callBackID);

    /// Fill tag with a unique tag for this client.
    void makeUniqueTag(char * tag);

    /// Simulate an Urbi message.
    virtual void notifyCallbacks(const UMessage &msg);

    /// Notify of an error.
    virtual void printf(const char * format, ...) = 0;

    /// Get time in milliseconds since an unspecified but constant
    /// reference time.
    virtual unsigned int getCurrentTime() const = 0;

    /** Set interval between ping messages in milliseconds, 0�to disable.
     Sends an URBI message at specified interval, if no anwser is received after
     three times this interval, close the connection.
    */
    virtual void setPingInterval(unsigned int msTime) = 0;

    /// Return the server name or IP address.
    const char * getServerName() {return host;}

    /// Called each time new data is available in recvBuffer.
    void processRecvBuffer();

    std::ostream & getStream() { return *stream;}

     /// dummy tag for client error callback
    static const char * CLIENTERROR_TAG;
  protected:
    /// Queue data for sending, returns zero on success, nonzero on failure.
    virtual int effectiveSend(const void * buffer, int size)=0;

    /// Whether successive effectiveSend() of cumulated size 'size'
    /// will succeed.
    virtual bool canSend(int size)=0;

    libport::Lockable& sendBufferLock;
    libport::Lockable& listLock;

    /// Add a callback to the list.
    UCallbackID addCallback(const char * tag, UCallbackWrapper &w);

    /// Generate a client error message and notify callbacks
    void clientError(const char * msg=0, int code=0);

    /// Host name.
    char	     *host;
    /// Urbi Port.
    int	     port;
    /// Urbi Buffer length.
    int	     buflen;
    /// System calls return value storage.
    int	     rc;

    /// Reception buffer.
    char	     *recvBuffer;
    /// Current position in reception buffer.
    int	     recvBufferPosition;
    /// Temporary buffer for send data.
    char	     *sendBuffer;


  private:
    /// Bin object for this command.
    std::list<urbi::BinaryData> bins;
    /// Temporary storage of binary data.
    void *binaryBuffer;
    /// Current position in binaryBuffer.
    int binaryBufferPosition;
    /// Size of binaryBuffer.
    int binaryBufferLength;

    /// Position of parse in recvBuffer.
    int	parsePosition;
    /// True if preparsing is in a string.
    bool inString;
    /// Current depth of bracket.
    int	nBracket;
    /// Start of command, after [ts:tag] header.
    char *currentCommand;

    /// Currently parsing binary
    bool binaryMode;

    /// Parsing a system message
    bool system;

    /// Position of end of header.
    int	endOfHeaderPosition;
    char currentTag[URBI_MAX_TAG_LENGTH];

    int	currentTimestamp;

    std::list<UCallbackInfo>callbackList;
    /// Unique tag base.
    int uid;

    std::ostream     *stream;

    friend class UClientStreambuf;

  };

  class UCallbackWrapper
  {
  public:
    virtual UCallbackAction operator ()(const UMessage &)=0;
    virtual ~UCallbackWrapper() {}
  };


  template<class elementT>
  class ElementTraits
  {
  public:
    typedef elementT	Element;
    typedef elementT& ElementReference;
    typedef const elementT& ConstElementReference;
  };

  template<class elementT>
  class ElementTraits<elementT&>
  {
  public:
    typedef elementT	Element;
    typedef elementT& ElementReference;
    typedef const elementT& ConstElementReference;
  };



  class UCallbackWrapperF
    : public UCallbackWrapper
  {
    UCallback cb;
  public:
    UCallbackWrapperF(UCallback cb) : cb(cb) {}
    virtual UCallbackAction operator ()(const UMessage & msg)
    {
      return cb(msg);
    }
    virtual ~UCallbackWrapperF() {}
  };

  class UCallbackWrapperCF
    : public UCallbackWrapper
  {
    UCustomCallback cb;
    void * cbData;
  public:
    UCallbackWrapperCF(UCustomCallback cb, void * cbData)
      : cb(cb), cbData(cbData)
    {}
    virtual UCallbackAction operator ()(const UMessage & msg)
    {
      return cb(cbData, msg);
    }
    virtual ~UCallbackWrapperCF() {}
  };

  template<class C>
  class UCallbackWrapper0 :
    public UCallbackWrapper
  {
    C& instance;
    UCallbackAction (C::*func)(const UMessage &);
  public:
    UCallbackWrapper0(C& instance, UCallbackAction (C::*func)(const UMessage &))
      : instance(instance), func(func)
    {}
    virtual UCallbackAction operator ()(const UMessage & msg)
    {
      return (instance.*func)(msg);
    }
    virtual ~UCallbackWrapper0() {}
  };

  template<class C, class P1>
  class UCallbackWrapper1
    : public UCallbackWrapper
  {
    C& instance;
    UCallbackAction (C::*funcPtr)(P1, const UMessage &);
    typename ElementTraits<P1>::Element  p1;
  public:
    UCallbackWrapper1(C& instance, UCallbackAction (C::*func)(P1, const UMessage &), P1 p1)
      : instance(instance), funcPtr(func), p1(p1) {}
    virtual UCallbackAction operator ()(const UMessage & msg) {return (instance.*funcPtr)(p1, msg);}
    virtual ~UCallbackWrapper1() {}
  };

  template<class C, class P1, class P2> class UCallbackWrapper2 : public UCallbackWrapper
  {
    C& instance;
    UCallbackAction (C::*func)(P1, P2, const UMessage &);
    typename ElementTraits<P1>::Element  p1;
    typename ElementTraits<P2>::Element  p2;
  public:
    UCallbackWrapper2(C& instance, UCallbackAction (C::*func)(P1, P2, const UMessage &), P1 p1, P2 p2)
      : instance(instance), func(func), p1(p1), p2(p2) {}
    virtual UCallbackAction operator ()(const UMessage & msg) {return (instance.*func)(p1, p2, msg);}
    virtual ~UCallbackWrapper2() {}
  };


  template<class C, class P1, class P2, class P3> class UCallbackWrapper3 : public UCallbackWrapper
  {
    C& instance;
    UCallbackAction (C::*func)(P1, P2, P3, const UMessage &);
    typename ElementTraits<P1>::Element  p1;
    typename ElementTraits<P2>::Element  p2;
    typename ElementTraits<P3>::Element  p3;
  public:
    UCallbackWrapper3(C& instance, UCallbackAction (C::*func)(P1, P2, P3, const UMessage &), P1 p1, P2 p2, P3 p3)
      : instance(instance), func(func), p1(p1), p2(p2), p3(p3) {}
    virtual UCallbackAction operator ()(const UMessage & msg) {return (instance.*func)(p1, p2, p3, msg);}
    virtual ~UCallbackWrapper3() {}
  };


  template<class C, class P1, class P2, class P3, class P4> class UCallbackWrapper4 : public UCallbackWrapper
  {
    C& instance;
    UCallbackAction (C::*func)(P1, P2, P3, P4, const UMessage &);
    typename ElementTraits<P1>::Element  p1;
    typename ElementTraits<P2>::Element  p2;
    typename ElementTraits<P3>::Element  p3;
    typename ElementTraits<P4>::Element  p4;
  public:
    UCallbackWrapper4(C& instance, UCallbackAction (C::*func)(P1, P2, P3, P4, const UMessage &), P1 p1, P2 p2, P3 p3, P4 p4)
      : instance(instance), func(func), p1(p1), p2(p2), p3(p3), p4(p4) {}
    virtual UCallbackAction operator ()(const UMessage & msg) {return (instance.*func)(p1, p2, p3, p4, msg);}
    virtual ~UCallbackWrapper4() {}
  };

  //overloaded callback generators

  inline UCallbackWrapper& callback(UCallback cb)
  {
    return *new UCallbackWrapperF(cb);
  }
  inline UCallbackWrapper& callback(UCustomCallback cb, void * data)
  {
    return *new UCallbackWrapperCF(cb, data);
  }


  template<class C>					     UCallbackWrapper& callback(C& ref,
											UCallbackAction (C::*func)(		    const UMessage &)	  )
  {
    return *new UCallbackWrapper0<C>(ref, func);
  }

  template<class C, class P1>				     UCallbackWrapper& callback(C& ref,
											UCallbackAction (C::*func)(P1,		    const UMessage &), P1 p1)
  {
    return *new UCallbackWrapper1<C, P1>(ref, func, p1);
  }

  template<class C, class P1, class P2>			     UCallbackWrapper& callback(C& ref,
											UCallbackAction (C::*func)(P1, P2,	     const UMessage &), P1 p1, P2 p2)
  {
    return *new UCallbackWrapper2<C, P1, P2>(ref, func, p1, p2);
  }

  template<class C, class P1, class P2, class P3>	     UCallbackWrapper& callback(C& ref,
											UCallbackAction (C::*func)(P1, P2, P3,		 const UMessage &), P1 p1, P2 p2, P3 p3)
  {
    return *new UCallbackWrapper3<C, P1, P2, P3>(ref, func, p1, p2, p3);
  }

  template<class C, class P1, class P2, class P3, class P4>  UCallbackWrapper& callback(C& ref,
											UCallbackAction (C::*func)(P1, P2, P3, P4,	     const UMessage &), P1 p1, P2 p2, P3 p3, P4 p4)
  {
    return *new UCallbackWrapper4<C, P1, P2, P3, P4>(ref, func, p1, p2, p3, p4);
  }




  //old-style addCallback, deprecated
  template<class C>					     UCallbackID UAbstractClient::setCallback(C& ref,
												      UCallbackAction (C::*func)(		  const UMessage &),		     const char * tag)
  {
    return addCallback(tag, *new UCallbackWrapper0<C>(ref, func));
  }

  template<class C, class P1>				    UCallbackID UAbstractClient::setCallback(C& ref,
												     UCallbackAction (C::*func)(P1 ,		 const UMessage &), P1 p1,	    const char * tag)
  {
    return addCallback(tag, *new UCallbackWrapper1<C, P1>(ref, func, p1));
  }

  template<class C, class P1, class P2>			    UCallbackID UAbstractClient::setCallback(C& ref,
												     UCallbackAction (C::*func)(P1 , P2,	 const UMessage &), P1 p1, P2 p2,	  const char * tag)
  {
    return addCallback(tag, *new UCallbackWrapper2<C, P1, P2>(ref, func, p1, p2));
  }

  template<class C, class P1, class P2, class P3>	    UCallbackID UAbstractClient::setCallback(C& ref,
												     UCallbackAction (C::*func)(P1 , P2, P3,	 const UMessage &), P1 p1 , P2 p2, P3 p3,     const char * tag)
  {
    return addCallback(tag, *new UCallbackWrapper3<C, P1, P2, P3>(ref, func, p1, p2, p3));
  }

  template<class C, class P1, class P2, class P3, class P4> UCallbackID UAbstractClient::setCallback(C& ref,
												     UCallbackAction (C::*func)(P1 , P2, P3, P4, const UMessage &), P1 p1, P2 p2, P3 p3, P4 p4, const char * tag)
  {
    return addCallback(tag, *new UCallbackWrapper4<C, P1, P2, P3, P4>(ref, func, p1, p2, p3, p4));
  }


  /// Conveniant macro for easy insertion of URBI code in C
  /**
     With this macro, the following code is enough to send a simple command to a robot using URBI:
     int main()
     {
     urbi::connect("robot");
     URBI(
     headPan.val'n = 0 time:1000 | headTilt.val'n = 0 time:1000, speaker.play("test.wav"),
     echo "test";
     );


     }

     The following construct is also valid:
     URBI() << "headPan.val="<<12<<";";
  */

# ifdef URBI
#  undef URBI
# endif

# if defined LIBURBI_NOARMOR
#  if !defined __GNUC__
#   error "as far as we know, your compiler does not support the __VA_ARGS__ C preprocessor extension, feature unavailable"
#  else
#   define URBI(...)							\
  (::urbi::getDefaultClient() ? *urbi::getDefaultClient() : std::cerr) << # __VA_ARGS__
#  endif
# else
#  define URBI(A)				\
  ::urbi::unarmorAndSend(#A)
# endif

  static const char semicolon = ';';
  static const char pipe = '|';
  static const char parallel = '&';
  static const char comma = ',';

  /// Must be called at the last line of your main() function.
  void execute(void);
  /// Terminate your URBI program.
  void exit(int code);
  /// Create a new UClient object
  UClient& connect(const char * host);
  /// Return the first UClient created by the program. Used by the URBI macro
  UClient* getDefaultClient();
  /// Redefine the default client
  void setDefaultClient(UClient * cl);
# ifndef DISABLE_IOSTREAM
  /// Send a possibly armored string to the default client
  std::ostream& unarmorAndSend(const char * str);
# endif
  extern UClient* defaultClient;

} // namespace urbi

#endif // URBI_UABSTRACTCLIENT_HH
