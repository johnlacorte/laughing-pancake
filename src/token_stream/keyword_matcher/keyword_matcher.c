#include "token_types.h"
#include "keyword_tokens.h"

int keyword_matcher(char *token_buffer)
{
  switch(token_buffer[0])
  {
    case 34:
    switch(token_buffer[1])
    {
      case 0:
      return TOKEN_DOUBLE_QUOTE;

      default:
      return TOKEN_NO_MATCH;
    }

    case 40:
    switch(token_buffer[1])
    {
      case 0:
      return TOKEN_OPEN_PAREN;

      default:
      return TOKEN_NO_MATCH;
    }

    case 41:
    switch(token_buffer[1])
    {
      case 0:
      return TOKEN_CLOSE_PAREN;

      default:
      return TOKEN_NO_MATCH;
    }

    case 91:
    switch(token_buffer[1])
    {
      case 0:
      return TOKEN_OPEN_SQUARE_BRACKET;

      default:
      return TOKEN_NO_MATCH;
    }

    case 109:
    switch(token_buffer[1])
    {
      case 111:
      switch(token_buffer[2])
      {
        case 100:
        switch(token_buffer[3])
        {
          case 117:
          switch(token_buffer[4])
          {
            case 108:
            switch(token_buffer[5])
            {
              case 101:
              switch(token_buffer[6])
              {
                case 0:
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

