#ifndef __STM32F7xx_H
#define __STM32F7xx_H

#ifdef __cplusplus
	extern "C" {
#endif
  
#if !defined (STM32F7)
#define STM32F7
#endif

#if !defined (STM32F756xx) && !defined (STM32F746xx) && !defined (STM32F745xx) && !defined (STM32F767xx) && \
    !defined (STM32F769xx) && !defined (STM32F777xx) && !defined (STM32F779xx) && !defined (STM32F722xx) && \
	!defined (STM32F723xx) && !defined (STM32F732xx) && !defined (STM32F733xx)
	/* #define STM32F756xx */	/* STM32F756VG, STM32F756ZG, STM32F756ZG, STM32F756IG, STM32F756BG, STM32F756NG Devices */
	/* #define STM32F746xx */	/* STM32F746VE, STM32F746VG, STM32F746ZE, STM32F746ZG, STM32F746IE, STM32F746IG, STM32F746BE, STM32F746BG, STM32F746NE, STM32F746NG Devices */
	/* #define STM32F745xx */	/* STM32F745VE, STM32F745VG, STM32F745ZG, STM32F745ZE, STM32F745IE, STM32F745IG Devices */
	/* #define STM32F765xx */	/* STM32F765BI, STM32F765BG, STM32F765NI, STM32F765NG, STM32F765II, STM32F765IG, STM32F765ZI, STM32F765ZG, STM32F765VI, STM32F765VG Devices */
	#define STM32F767xx			/* STM32F767BG, STM32F767BI, STM32F767IG, STM32F767II, STM32F767NG, STM32F767NI, STM32F767VG, STM32F767VI, STM32F767ZG, STM32F767ZI Devices */
	/* #define STM32F769xx */	/* STM32F769AG, STM32F769AI, STM32F769BG, STM32F769BI, STM32F769IG, STM32F769II, STM32F769NG, STM32F769NI, STM32F768AI Devices */
	/* #define STM32F777xx */	/* STM32F777VI, STM32F777ZI, STM32F777II, STM32F777BI, STM32F777NI Devices */
	/* #define STM32F779xx */	/* STM32F779II, STM32F779BI, STM32F779NI, STM32F779AI, STM32F778AI Devices */
	/* #define STM32F722xx */	/* STM32F722IE, STM32F722ZE, STM32F722VE, STM32F722RE, STM32F722IC, STM32F722ZC, STM32F722VC, STM32F722RC Devices */
	/* #define STM32F723xx */	/* STM32F723IE, STM32F723ZE, STM32F723VE, STM32F723IC, STM32F723ZC, STM32F723VC Devices */
	/* #define STM32F732xx */	/* STM32F732IE, STM32F732ZE, STM32F732VE, STM32F732RE Devices */
	/* #define STM32F733xx */	/* STM32F733IE, STM32F733ZE, STM32F733VE Devices */
#endif

#if defined(STM32F722xx)
  #include "stm32f722xx.h"
#elif defined(STM32F723xx)
  #include "stm32f723xx.h"
#elif defined(STM32F732xx)
  #include "stm32f732xx.h"
#elif defined(STM32F733xx)
  #include "stm32f733xx.h"    
#elif defined(STM32F756xx)
  #include "stm32f756xx.h"
#elif defined(STM32F746xx)
  #include "stm32f746xx.h"
#elif defined(STM32F745xx)
  #include "stm32f745xx.h"
#elif defined(STM32F765xx)
  #include "stm32f765xx.h"
#elif defined(STM32F767xx)
  #include "stm32f767xx.h"
#elif defined(STM32F769xx)
  #include "stm32f769xx.h"
#elif defined(STM32F777xx)
  #include "stm32f777xx.h"
#elif defined(STM32F779xx)
  #include "stm32f779xx.h"  
#else
 #error "Please select first the target STM32F7xx device used in your application (in stm32f7xx.h file)"
#endif

typedef enum {
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum {
  DISABLE = 0, 
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {
  ERROR = 0, 
  SUCCESS = !ERROR
} ErrorStatus;

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 

#ifdef __cplusplus
}
#endif

#endif

