

#pragma once

// splits a string at the first equal sign.
Result ParseNameValueString(const string& in, string& name, string& value)
{
  string::size_type nEq = in.find('=');
  if(nEq == string::npos)
  {
		return Result::Failure("Invalid value... no equal sign was found.");
  }
  name = in.substr(0, nEq);
  value = in.c_str() + nEq + 1;// i'm not sure you can substr() at the null-term, so i'll do it this way where it's guaranteed to be safe
  return Result::Success();
}


// decimal value.  make sure there's no crap though.
template<int base>
inline Result ParseXBaseString(const string& s, DWORD& out)
{
  char* pEnd;
  out = strtoul(s.c_str(), &pEnd, base);
  if(*pEnd != 0)
  {
		return Result::Failure(Format("Error parsing base % string %.  Invalid characters were found.").i(base).qs(s).Str());
  }
	return Result::Success();
}

// either 0xnnnnnn or nnnnn
inline Result ParseNonBraketedString(const string& s, DWORD& out)
{
  if(s.length() < 3)// shortest hex value is 0x0
  {
    return ParseXBaseString<10>(s, out);
  }
  // is it decimal or hex?
  if(s[0] == '0' && s[1] == 'x')
  {
    return ParseXBaseString<16>(s, out);
  }
  return ParseXBaseString<10>(s, out);
}

bool FitsInAWord(DWORD dw)
{
  return dw < 0x10000;
}

bool FitsInAByte(DWORD dw)
{
  return dw < 0x100;
}

template<char closingBrace, int base>
inline Result ParseBraketedXBaseString(const string& s, DWORD& out)
{
  if((*s.rbegin()) != closingBrace)
  {
		return Result::Failure(Format("Closing '%' was not found.").c(closingBrace).Str());
  }
  string inner = s.substr(1, s.length() - 2);// extract inner values
  vector<string> valueStrings;
  LibCC::StringSplit(inner, " ", std::back_inserter(valueStrings));
  vector<DWORD> values;
  if(valueStrings.size() != 2 && valueStrings.size() != 4 && valueStrings.size() != 1)
  {
		return Result::Failure(Format("Invalid number of values specified (expected: 1, 2, or 4. actual: %).")((int)valueStrings.size()).Str());
  }
  for(vector<string>::iterator it = valueStrings.begin(); it != valueStrings.end(); ++ it)
  {
    Result res;
    DWORD temp;
    if(!(res = ParseXBaseString<base>(*it, temp)))
    {
      return res.Prepend(Format("Error parsing bracketed base % string; ").i(base).Str());
    }
    values.push_back(temp);
  }
  switch(valueStrings.size())
  {
  case 1:
    // [0]
    out = values[0];
    break;
  case 2:
    // [0 1] - each part must fit in a word
    if(!FitsInAWord(values[0]))
    {
			return Result::Failure("First value is too large");
    }
    if(!FitsInAWord(values[1]))
    {
			return Result::Failure("Second value is too large");
    }
    out = (values[0] << 16) | values[1];
    break;
  case 4:
    // [0 1 2 3] - each part must fit in a byte
    if(!FitsInAByte(values[0]))
    {
      return Result::Failure("First value is too large");
    }
    if(!FitsInAByte(values[1]))
    {
      return Result::Failure("Second value is too large");
    }
    if(!FitsInAByte(values[2]))
    {
      return Result::Failure("Third value is too large");
    }
    if(!FitsInAByte(values[3]))
    {
      return Result::Failure("Fourth value is too large");
    }
    out = (values[0] << 24) | (values[1] << 16) | (values[2] << 8) | values[3];
    break;
  }
  return Result(S_OK);
}

inline Result ParseDWORDString(const string& s, DWORD& out)
{
  Result res;
  if(s.length() == 0)
  {
		return Result::Failure("The string is empty.");
  }
  if(s.length() < 3)// shortest braketed value is [0]
  {
    return ParseNonBraketedString(s, out);
  }
  // is it braketed or not?
  if(s[0] == '[')
  {
    return ParseBraketedXBaseString<']', 16>(s, out);
  }
  if(s[0] == '{')
  {
    return ParseBraketedXBaseString<'}', 10>(s, out);
  }
  return ParseNonBraketedString(s, out);
}


