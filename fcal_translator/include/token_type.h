#ifndef PROJECT_INCLUDE_TOKEN_TYPE_H_
#define PROJECT_INCLUDE_TOKEN_TYPE_H_

namespace fcal {
namespace scanner {

enum kTokenEnumType {
    /*0*/ kIntKwd,
    kFloatKwd,
    kStringKwd,
    kMatrixKwd,
    /*4*/ kLetKwd,
    kInKwd,
    kEndKwd,
    kIfKwd,
    kThenKwd,
    kElseKwd,
    /*10*/ kRepeatKwd,
    kPrintKwd,
    kToKwd,

    // Constants
    /*13*/ kIntConst, kFloatConst,
    kStringConst,

    // Names
    /*16*/ kVariableName,

    // Punctuation
    /*17*/ kLeftParen,
    kRightParen,
    kLeftCurly,
    kRightCurly,
    /*21*/ kLeftSquare,
    kRightSquare,

    kSemiColon,
    kColon,

    // Operators
    /*25*/ kAssign,
    kPlusSign,
    kStar,
    kDash,
    kForwardSlash,
    /*30*/ kLessThan,
    kLessThanEqual,
    kGreaterThan,
    kGreaterThanEqual,
    /*34*/ kEqualsEquals,
    kNotEquals,
    /*36*/ kAndOp,
    kOrOp,
    kNotOp,

    // Special terminal types
    /*39*/ kEndOfFile,
    kLexicalError
}; /* kTokenEnumType */

typedef enum kTokenEnumType TokenType;

} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_TOKEN_TYPE_H_
