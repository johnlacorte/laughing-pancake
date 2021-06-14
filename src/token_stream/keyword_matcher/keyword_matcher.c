#include "keyword_tokens.h"
#include "opcode_tokens.h"

int keyword_matcher(char *token_buffer)
{
  switch(token_buffer[0])
  {
    case '(':
    switch(token_buffer[1])
    {
        case '\0':
        return TOKEN_OPEN_PAREN;

        default:
        return TOKEN_NO_MATCH;
    }

    case ')':
    switch(token_buffer[1])
    {
        case '\0':
        return TOKEN_CLOSE_PAREN;

        default:
        return TOKEN_NO_MATCH;
    }

    case 'm':
    switch(token_buffer[1])
    {
      case 'o':
      switch(token_buffer[2])
      {
        case 'd':
        switch(token_buffer[3])
        {
          case 'u':
          switch(token_buffer[4])
          {
            case 'l':
            switch(token_buffer[5])
            {
              case 'e':
              switch(token_buffer[6])
              {
                case '\0':
                return TOKEN_KEYWORD_MODULE;

                default:
                return TOKEN_NO_MATCH;
              }

              default:
              return TOKEN_NO_MATCH;
            }

            default:
            return TOKEN_NO_MATCH;
          }

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      default:
      return TOKEN_NO_MATCH;
    }

    default:
    return TOKEN_NO_MATCH;
  }
}