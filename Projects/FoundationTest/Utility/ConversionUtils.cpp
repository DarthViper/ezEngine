#include <PCH.h>
#include <Foundation/Utilities/ConversionUtils.h>

EZ_CREATE_SIMPLE_TEST_GROUP(Utility);

EZ_CREATE_SIMPLE_TEST(Utility, ConversionUtils)
{
  EZ_TEST_BLOCK(ezTestBlock::Enabled, "StringToInt")
  {
    const char* szString = "1a";
    const char* szResultPos = NULL;

    ezInt32 iRes = 42;
    szString = "01234";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 1234);
    EZ_TEST(szResultPos == szString + 5);

    iRes = 42;
    szString = "0";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 0);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "0000";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 0);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "-999999";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, -999999);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "-+999999";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, -999999);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "--999999";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 999999);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "++---+--+--999999";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, -999999);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "++--+--+--999999";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 999999);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "123+456";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 123);
    EZ_TEST(szResultPos == szString + 3);

    iRes = 42;
    szString = "123_456";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 123);
    EZ_TEST(szResultPos == szString + 3);

    iRes = 42;
    szString = "-123-456";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, -123);
    EZ_TEST(szResultPos == szString + 4);


    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt(NULL, iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt("", iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt("a", iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt("a15", iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt("+", iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    EZ_TEST(ezConversionUtils::StringToInt("-", iRes) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);

    iRes = 42;
    szString = "1a";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 1);
    EZ_TEST(szResultPos == szString + 1);

    iRes = 42;
    szString = "0 23";
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 0);
    EZ_TEST(szResultPos == szString + 1);

    // overflow check

    iRes = 42;
    szString = "0002147483639"; // valid
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_INT(iRes, 2147483639);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    iRes = 42;
    szString = "0002147483640"; // invalid
    EZ_TEST(ezConversionUtils::StringToInt(szString, iRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST_INT(iRes, 42);
  }

  EZ_TEST_BLOCK(ezTestBlock::Enabled, "StringToFloat")
  {
    const char* szString = NULL;
    const char* szResultPos = NULL;

    double fRes = 42;
    szString =  "23.45f";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 23.45, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    fRes = 42;
    szString =  "-2345f";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, -2345.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));

    fRes = 42;
    szString =  "-0";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 0.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  "00000.000000f";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 0.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  ".123456789";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 0.123456789, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  "+123e1";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 1230.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  "  \r\t 123e0";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 123.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  "\n123e6";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 123000000.0, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  "  123e-6f";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 0.000123, 0.00001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  " + - -+-123.45e-10f";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, -0.000000012345, 0.0000001);
    EZ_TEST(szResultPos == szString + ezStringUtils::GetStringElementCount(szString));  

    fRes = 42;
    szString =  NULL;
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST_DOUBLE(fRes, 42.0, 0.00001);

    fRes = 42;
    szString =  "";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST_DOUBLE(fRes, 42.0, 0.00001);

    fRes = 42;
    szString =  "-----";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST_DOUBLE(fRes, 42.0, 0.00001);

    fRes = 42;
    szString =  " + - +++ - \r \n";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST_DOUBLE(fRes, 42.0, 0.00001);


    fRes = 42;
    szString =  "65.345789xabc";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, 65.345789, 0.000001);
    EZ_TEST(szResultPos == szString + 9);  

    fRes = 42;
    szString =  " \n \r \t + - 2314565.345789ff xabc";
    EZ_TEST(ezConversionUtils::StringToFloat(szString, fRes, &szResultPos) == EZ_SUCCESS);
    EZ_TEST_DOUBLE(fRes, -2314565.345789, 0.000001);
    EZ_TEST(szResultPos == szString + 26);  
  }

  EZ_TEST_BLOCK(ezTestBlock::Enabled, "StringToBool")
  {
    const char* szString = "";
    const char* szResultPos = NULL;
    bool bRes = false;

    // true / false
    {
      bRes = false;
      szString = "true,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(bRes);
      EZ_TEST(*szResultPos == ',');

      bRes = true;
      szString = "FALSe,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(!bRes);
      EZ_TEST(*szResultPos == ',');
    }

    // on / off
    {
      bRes = false;
      szString = "\n on,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(bRes);
      EZ_TEST(*szResultPos == ',');

      bRes = true;
      szString = "\t\t \toFf,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(!bRes);
      EZ_TEST(*szResultPos == ',');
    }

    // 1 / 0
    {
      bRes = false;
      szString = "\r1,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(bRes);
      EZ_TEST(*szResultPos == ',');

      bRes = true;
      szString = "0,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(!bRes);
      EZ_TEST(*szResultPos == ',');
    }

    // yes / no
    {
      bRes = false;
      szString = "yes,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(bRes);
      EZ_TEST(*szResultPos == ',');

      bRes = true;
      szString = "NO,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(!bRes);
      EZ_TEST(*szResultPos == ',');
    }

    // enable / disable
    {
      bRes = false;
      szString = "enable,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(bRes);
      EZ_TEST(*szResultPos == ',');

      bRes = true;
      szString = "disABle,";
      szResultPos = NULL;
      EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_SUCCESS);
      EZ_TEST(!bRes);
      EZ_TEST(*szResultPos == ',');
    }

    bRes = false;

    szString = "of,";
    szResultPos = NULL;
    EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST(szResultPos == NULL);

    szString = "aon";
    szResultPos = NULL;
    EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST(szResultPos == NULL);

    szString = "";
    szResultPos = NULL;
    EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST(szResultPos == NULL);

    szString = NULL;
    szResultPos = NULL;
    EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST(szResultPos == NULL);

    szString = "tut";
    szResultPos = NULL;
    EZ_TEST(ezConversionUtils::StringToBool(szString, bRes, &szResultPos) == EZ_FAILURE);
    EZ_TEST(szResultPos == NULL);

  }
}


