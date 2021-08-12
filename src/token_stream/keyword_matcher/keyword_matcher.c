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

    case 61:
    switch(token_buffer[1])
    {
      case 0:
      return TOKEN_EQUAL_SIGN;

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

    case 99:
    switch(token_buffer[1])
    {
      case 111:
      switch(token_buffer[2])
      {
        case 100:
        switch(token_buffer[3])
        {
          case 101:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_CODE;

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

    case 100:
    switch(token_buffer[1])
    {
      case 97:
      switch(token_buffer[2])
      {
        case 116:
        switch(token_buffer[3])
        {
          case 97:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_DATA;

            default:
            return TOKEN_NO_MATCH;
          }

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 101:
      switch(token_buffer[2])
      {
        case 99:
        switch(token_buffer[3])
        {
          case 108:
          switch(token_buffer[4])
          {
            case 97:
            switch(token_buffer[5])
            {
              case 114:
              switch(token_buffer[6])
              {
                case 101:
                switch(token_buffer[7])
                {
                  case 0:
                  return TOKEN_KEYWORD_DECLARE;

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

    case 101:
    switch(token_buffer[1])
    {
      case 108:
      switch(token_buffer[2])
      {
        case 101:
        switch(token_buffer[3])
        {
          case 109:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_ELEM;

            default:
            return TOKEN_NO_MATCH;
          }

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 120:
      switch(token_buffer[2])
      {
        case 112:
        switch(token_buffer[3])
        {
          case 111:
          switch(token_buffer[4])
          {
            case 114:
            switch(token_buffer[5])
            {
              case 116:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_EXPORT;

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

        case 116:
        switch(token_buffer[3])
        {
          case 101:
          switch(token_buffer[4])
          {
            case 114:
            switch(token_buffer[5])
            {
              case 110:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_EXTERN;

                case 114:
                switch(token_buffer[7])
                {
                  case 101:
                  switch(token_buffer[8])
                  {
                    case 102:
                    switch(token_buffer[9])
                    {
                      case 0:
                      return TOKEN_KEYWORD_EXTERNREF;

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

        default:
        return TOKEN_NO_MATCH;
      }

      default:
      return TOKEN_NO_MATCH;
    }

    case 102:
    switch(token_buffer[1])
    {
      case 51:
      switch(token_buffer[2])
      {
        case 50:
        switch(token_buffer[3])
        {
          case 0:
          return TOKEN_KEYWORD_F32;

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 54:
      switch(token_buffer[2])
      {
        case 52:
        switch(token_buffer[3])
        {
          case 0:
          return TOKEN_KEYWORD_F64;

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 117:
      switch(token_buffer[2])
      {
        case 110:
        switch(token_buffer[3])
        {
          case 99:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_FUNC;

            case 114:
            switch(token_buffer[5])
            {
              case 101:
              switch(token_buffer[6])
              {
                case 102:
                switch(token_buffer[7])
                {
                  case 0:
                  return TOKEN_KEYWORD_FUNCREF;

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

    case 103:
    switch(token_buffer[1])
    {
      case 108:
      switch(token_buffer[2])
      {
        case 111:
        switch(token_buffer[3])
        {
          case 98:
          switch(token_buffer[4])
          {
            case 97:
            switch(token_buffer[5])
            {
              case 108:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_GLOBAL;

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

    case 105:
    switch(token_buffer[1])
    {
      case 51:
      switch(token_buffer[2])
      {
        case 50:
        switch(token_buffer[3])
        {
          case 0:
          return TOKEN_KEYWORD_I32;

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 54:
      switch(token_buffer[2])
      {
        case 52:
        switch(token_buffer[3])
        {
          case 0:
          return TOKEN_KEYWORD_I64;

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      case 109:
      switch(token_buffer[2])
      {
        case 112:
        switch(token_buffer[3])
        {
          case 111:
          switch(token_buffer[4])
          {
            case 114:
            switch(token_buffer[5])
            {
              case 116:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_IMPORT;

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

      case 116:
      switch(token_buffer[2])
      {
        case 101:
        switch(token_buffer[3])
        {
          case 109:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_ITEM;

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

    case 108:
    switch(token_buffer[1])
    {
      case 111:
      switch(token_buffer[2])
      {
        case 99:
        switch(token_buffer[3])
        {
          case 97:
          switch(token_buffer[4])
          {
            case 108:
            switch(token_buffer[5])
            {
              case 0:
              return TOKEN_KEYWORD_LOCAL;

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

    case 109:
    switch(token_buffer[1])
    {
      case 101:
      switch(token_buffer[2])
      {
        case 109:
        switch(token_buffer[3])
        {
          case 111:
          switch(token_buffer[4])
          {
            case 114:
            switch(token_buffer[5])
            {
              case 121:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_MEMORY;

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

      case 117:
      switch(token_buffer[2])
      {
        case 116:
        switch(token_buffer[3])
        {
          case 0:
          return TOKEN_KEYWORD_MUT;

          default:
          return TOKEN_NO_MATCH;
        }

        default:
        return TOKEN_NO_MATCH;
      }

      default:
      return TOKEN_NO_MATCH;
    }

    case 110:
    switch(token_buffer[1])
    {
      case 97:
      switch(token_buffer[2])
      {
        case 109:
        switch(token_buffer[3])
        {
          case 101:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_NAME;

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

    case 111:
    switch(token_buffer[1])
    {
      case 102:
      switch(token_buffer[2])
      {
        case 102:
        switch(token_buffer[3])
        {
          case 115:
          switch(token_buffer[4])
          {
            case 101:
            switch(token_buffer[5])
            {
              case 116:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_OFFSET;

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

    case 112:
    switch(token_buffer[1])
    {
      case 97:
      switch(token_buffer[2])
      {
        case 114:
        switch(token_buffer[3])
        {
          case 97:
          switch(token_buffer[4])
          {
            case 109:
            switch(token_buffer[5])
            {
              case 0:
              return TOKEN_KEYWORD_PARAM;

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

    case 114:
    switch(token_buffer[1])
    {
      case 101:
      switch(token_buffer[2])
      {
        case 115:
        switch(token_buffer[3])
        {
          case 117:
          switch(token_buffer[4])
          {
            case 108:
            switch(token_buffer[5])
            {
              case 116:
              switch(token_buffer[6])
              {
                case 0:
                return TOKEN_KEYWORD_RESULT;

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

    case 115:
    switch(token_buffer[1])
    {
      case 116:
      switch(token_buffer[2])
      {
        case 97:
        switch(token_buffer[3])
        {
          case 114:
          switch(token_buffer[4])
          {
            case 116:
            switch(token_buffer[5])
            {
              case 0:
              return TOKEN_KEYWORD_START;

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

    case 116:
    switch(token_buffer[1])
    {
      case 97:
      switch(token_buffer[2])
      {
        case 98:
        switch(token_buffer[3])
        {
          case 108:
          switch(token_buffer[4])
          {
            case 101:
            switch(token_buffer[5])
            {
              case 0:
              return TOKEN_KEYWORD_TABLE;

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

      case 121:
      switch(token_buffer[2])
      {
        case 112:
        switch(token_buffer[3])
        {
          case 101:
          switch(token_buffer[4])
          {
            case 0:
            return TOKEN_KEYWORD_TYPE;

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

