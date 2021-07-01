// 10727130 黃意勛 10727124 劉宇廷

#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <vector>
#include <stack>
#include <iomanip>
using namespace std ;

typedef struct StudentType {
  char sid[10] = {'\0'} ;     // use '\0' to check whether it's empty.
  char sname[10] ;
  unsigned char score[6] ;
  float mean ;
};

typedef struct HashType {
  char sid[10] = {'\0'} ;
  char sname[10] ;
  unsigned char score[6] ;
  float mean ;
  int pos = 0 ;
  int hvalue = 0 ;
};

class Hash {
public:
string fileNum, fileName ;
fstream inFile, outFile ;     // if open, don't forget to close.
vector<StudentType> list ;
int SPM ;     // smallest prime number

bool text2Binary() {
  do {
    cout << "Input a file number([0]Quit): " ;
    cin >> fileNum ;

    if ( !fileNum.compare("0") ) {
      cout << endl ;
      return false ;
    }

    inFile.open( ( "input" + fileNum + ".bin" ).c_str(), fstream::in ) ;
    if ( inFile.is_open() ) {
      fileName = "input" + fileNum + ".bin" ;
      inFile.close() ;
      return true ;
    }

    inFile.open( ( "input" + fileNum + ".txt" ).c_str(), fstream::in ) ;
    if ( inFile.is_open() ) {
      cout << "\n### input" << fileNum << ".bin does not exit! ###\n" ;
      break ;
    }
  } while(true) ;

  fileName = "input" + fileNum + ".bin" ;
  outFile.open( fileName.c_str(), fstream::out | fstream::binary ) ;

  if ( outFile.is_open() ) {
    int stNo = 0 ;
    char rBuf[255] ;

    while( inFile.getline( rBuf, 255, '\n' ) ){
      string temp ;
      StudentType oneSt ;
      int cNo = 0, pre = 0, pos = 0 ;

      stNo++ ;
      temp.assign(rBuf) ;
      pos = temp.find_first_of( '\t', pre ) ;

      while( pos != string::npos ) {
        switch( ++cNo ) {
          case 1 : strcpy( oneSt.sid, temp.substr( pre, pos-pre ).c_str() ) ;
            break ;
          case 2 : strcpy( oneSt.sname, temp.substr( pre, pos-pre).c_str() ) ;
            break ;
          default : oneSt.score[cNo-3] = atoi( temp.substr( pre, pos-pre).c_str() ) ;
            break ;
        }
        pre = ++pos ;
        pos  = temp.find_first_of( '\t', pre ) ;
      }

      pos = temp.find_last_of('\t') ;
      oneSt.mean = atof( temp.substr(pos+1).c_str() ) ;
      outFile.write( (char*)&oneSt, sizeof(oneSt) ) ;
    }
    inFile.close() ;
    outFile.close() ;
  }
  return true ;
}

int findSPM() {
  bool check = false ;

  for ( int i = ( list.size() * 1.2 ) + 1 ; ; i++ ) {
    for ( int j = 2 ; j < i ; j++ ) {
      if ( i % j == 0 ) {
        check = true ;
      }
    }

    if ( check == false ) {
      return i ;
    }

    check = false ;
  }
}

void mission1() {
  list.clear() ;

  StudentType oneSt ;
  int stNo ;

  inFile.open( fileName.c_str(), fstream::in | fstream::binary ) ;

  if( inFile.is_open() ) {
    inFile.seekg( 0, inFile.end ) ;     // 0~eof.
    stNo = inFile.tellg() / sizeof(oneSt) ;
    inFile.seekg( 0, inFile.beg ) ;

    for( int i = 0 ; i < stNo ; i++ ) {
      inFile.read( (char*)&oneSt, sizeof(oneSt) ) ;
      list.push_back(oneSt) ;
    }
  }

  inFile.close() ;

  SPM = findSPM() ;
  HashType hashtable[SPM] ;

  float step = 0 ;
  float allstep = 0 ;

  for ( int i = 0 ; i < stNo ; i++ ) {
    long long int sum = 1 ;     // too big.
    int j = 0  ;

    while ( list.at(i).sid[j] != '\0' && j < 10 ) {
      sum = sum  * list.at(i).sid[j] ;
      j++;
    }

    sum = sum % SPM ;
    int hv = sum ;     // record hvalue
    allstep++ ;

    if ( hashtable[sum].sid[0] == '\0' ) {     // if there is empty, put in directly.
      strcpy( hashtable[sum].sid, list.at(i).sid ) ;
      strcpy( hashtable[sum].sname, list.at(i).sname ) ;
      for(int i = 0 ; i < 6 ; i++ ) {
        hashtable[sum].score[i] = list.at(i).score[i] ;
      }
      hashtable[sum].mean = list.at(i).mean ;
      hashtable[sum].pos = sum ;
      hashtable[sum].hvalue = hv ;
    }
    else {
      int k = 0, saved = sum ;     // saved = hvalue.
      while ( hashtable[sum].sid[0] != '\0' ) {
        k++ ;     // (1,2,3,...)^2
        allstep++ ;
        sum = saved + ( k*k ) ;

        if ( sum >= SPM ) {     // if the number bigger than hashtable size.
          sum = sum % SPM ;
        }
      }

      strcpy( hashtable[sum].sid, list.at(i).sid ) ;
      strcpy( hashtable[sum].sname, list.at(i).sname ) ;
      for( int i = 0 ; i < 6 ; i++ ) {
        hashtable[sum].score[i] = list.at(i).score[i] ;
      }
      hashtable[sum].mean = list.at(i).mean ;
      hashtable[sum].pos = sum ;
      hashtable[sum].hvalue = hv ;
    }
  }

  for ( int i = 0 ; i < SPM ; i++ ) {
    if ( hashtable[i].sid[0] == '\0' )
      ;
    else {
      for ( int j = 1 ; ; j++ ) {
        int z = i + (j*j) ;

        if ( z >= SPM ) {
          z = z % SPM ;
        }

        if ( hashtable[z].sid[0] == '\0' ) {     // find empty place.
          step = step + j ;
          break ;
        }
      }
    }
  }

  float unsuccess, success ;
  unsuccess = step / SPM ;
  success = allstep / list.size() ;

  cout << "\n*** Hash table has been successfully created by Quadratic probing. ***" << endl ;
  printf( "unsuccessful search: %6.4f comparisons on average\n", unsuccess ) ;
  printf( "successful search: %6.4f comparisons on average\n\n", success ) ;

  fileName = "quadratic" + fileNum + ".txt" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if ( outFile.is_open()) {
    outFile << " --- Hash table created by Quadratic probing ---\n" ;

    for( int i = 0 ; i < SPM ; i++ ) {
      outFile << "[" << setw(3) << i << "] " ;

      if ( hashtable[i].hvalue != 0 ) {
        outFile << setw(10) << hashtable[i].hvalue << ", " ;
        outFile << setw(10) << hashtable[i].sid << ", " ;
        outFile << setw(10) << hashtable[i].sname << ", " ;
        outFile << setw(10) << hashtable[i].mean << "\n" ;
      }
      else {
        outFile << "\n" ;
      }
    }

    outFile << " ----------------------------------------------------- \n" ;
    outFile.close() ;
  }

  string search ;
  do {
    cout << "Input a student ID to search ([0] Quit): " ;
    cin >> search ;

    if ( !search.compare("0") ) {
      cout << endl ;
      break ;
    }
    else {
      long long int sum = 1 ;     // too big.
      int j = 0, fish = 0, probe = 1 ;
      int savedfish = 0, savedpos = 0 ;
      bool found = false ;

      while ( j < search.length() ) {
        sum = sum  * search[j] ;
        j++;
      }
      savedfish = sum % SPM ;
      fish = savedfish ;

      j = 1 ;
      while ( hashtable[fish].sid[0] != '\0' && !found ) {
        if ( atoi( hashtable[fish].sid ) == atoi( search.c_str() ) ) {
          savedpos = fish ;
          found = true ;
          break ;
        }

        fish = savedfish + (j*j) ;
        if ( fish >= SPM ) {
          fish = fish % SPM ;
        }
        probe++ ;
        j++ ;
      }

      if ( !found ) {
        cout << search << " is not found after " << probe << " probes.\n\n" ;

      }
      else {
        cout << "{ " << hashtable[savedpos].sid << ", " << hashtable[savedpos].sname << ", "
          << hashtable[savedpos].mean << " } is found after " << probe << " probes.\n\n" ;
      }
    }
  } while(true) ;
}

int findSPM2() {
  bool check = false ;

  for ( int i = ( list.size() / 3 ) + 1 ; ; i++ ) {
    for ( int j = 2 ; j < i ; j++ ) {
      if ( i % j == 0 ) {
        check = true ;
      }
    }

    if ( check == false ) {
      return i ;
    }
    check = false ;
  }
}

void mission2() {
  int SPM2 = findSPM2() ;

  HashType hashtable[SPM] ;
  float allstep = 0 ;
  long long int savedsum ;

  for ( int i = 0 ; i < list.size() ; i++ ) {
    long long int sum = 1 ;
    int j = 0  ;

    while ( list.at(i).sid[j] != '\0' && j < 10 ) {
      sum = sum  * list.at(i).sid[j] ;
      j++;
    }

    savedsum = sum ;     // the step function needs it.
    sum = sum % SPM ;
    int hv = sum ;
    allstep++ ;

    if ( hashtable[sum].sid[0] == '\0' ) {
      strcpy( hashtable[sum].sid, list.at(i).sid ) ;
      strcpy( hashtable[sum].sname, list.at(i).sname ) ;
      for(int i = 0 ; i < 6 ; i++ ) {
        hashtable[sum].score[i] = list.at(i).score[i] ;
      }
      hashtable[sum].mean = list.at(i).mean ;
      hashtable[sum].pos = sum ;
      hashtable[sum].hvalue = hv ;
    }
    else {
      int step = SPM2 - ( savedsum % SPM2 ) ;

      while ( hashtable[sum].sid[0] != '\0' ) {
        allstep++ ;
        sum = sum + step;

        if ( sum >= SPM ) {
          sum = sum % SPM ;
        }
      }

      strcpy( hashtable[sum].sid, list.at(i).sid ) ;
      strcpy( hashtable[sum].sname, list.at(i).sname ) ;
      for( int i = 0 ; i < 6 ; i++ ) {
        hashtable[sum].score[i] = list.at(i).score[i] ;
      }
      hashtable[sum].mean = list.at(i).mean ;
      hashtable[sum].pos = sum ;
      hashtable[sum].hvalue = hv ;
    }
  }

  float success = allstep / list.size() ;

  cout << "\n*** Hash table has been successfully created by Double hashing. ***" << endl ;
  printf( "successful search: %6.4f comparisons on average\n\n", success ) ;

  fileName = "double" + fileNum + ".txt" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if ( outFile.is_open()) {
    outFile << " --- Hash table created by Double hashing    ---\n" ;

    for( int i = 0 ; i < SPM ; i++ ) {
      outFile << "[" << setw(3) << i << "] " ;

      if (hashtable[i].hvalue != 0 ) {
        outFile << setw(10) << hashtable[i].hvalue << ", " ;
        outFile << setw(10) << hashtable[i].sid << ", " ;
        outFile << setw(10) << hashtable[i].sname << ", " ;
        outFile << setw(10) << hashtable[i].mean << "\n" ;
      }
      else {
        outFile << "\n" ;
      }
    }

    outFile << " ----------------------------------------------------- \n" ;
    outFile.close() ;
  }

  string search ;
  do {
    cout << "Input a student ID to search ([0] Quit): " ;
    cin >> search ;

    if ( !search.compare("0") ) {
      cout << endl ;
      break ;
    }
    else {
      long long int sum = 1 ;     // too big.
      int j = 0, fish = 0, probe = 1, savedpos = 0 ;
      bool found = false ;

      while ( j < search.length() ) {
        sum = sum  * search[j] ;
        j++;
      }
      fish = sum % SPM ;

      int step = SPM2 - ( sum % SPM2 ) ;

      while ( hashtable[fish].sid[0] != '\0' && !found ) {
        if ( atoi( hashtable[fish].sid ) == atoi( search.c_str() ) ) {
          savedpos = fish ;
          found = true ;
          break ;
        }
        fish = fish + step ;
        if ( fish >= SPM ) {
          fish = fish % SPM ;
        }
        probe++ ;
        j++ ;
      }

      if ( !found ) {
        cout << search << " is not found after " << probe << " probes.\n\n" ;

      }
      else {
        cout << "{ " << hashtable[savedpos].sid << ", " << hashtable[savedpos].sname << ", "
          << hashtable[savedpos].mean << " } is found after " << probe << " probes.\n\n" ;
      }
    }
  } while(true) ;
}
};

int main() {
  string choice ;
  Hash hash ;

  cout << "*****   Hash Table   *****" << endl ;
  cout << "* 0. Quit                *" << endl ;
  cout << "* 1. Quadratic probing   *" << endl ;
  cout << "* 2. Double hashing      *" << endl ;
  cout << "**************************" << endl ;
  cout << "Input a choice(0, 1, 2): " ;
  cin >> choice ;

  while ( choice != "0" ) {
    if ( choice == "1" ) {
      if ( hash.text2Binary() ) {
        hash.mission1() ;
      }
    }
    else if ( choice == "2" ) {
      if ( hash.list.empty() ) {
        cout << "\n### Command 1 first. ###\n" << endl ;
      }
      else {
        hash.mission2() ;
      }
    }
    else {
      cout << endl << "Command does not exit!"  << endl << endl ;
    }

    cout << "*****   Hash Table   *****" << endl ;
    cout << "* 0. Quit                *" << endl ;
    cout << "* 1. Quadratic probing   *" << endl ;
    cout << "* 2. Double hashing      *" << endl ;
    cout << "**************************" << endl ;
    cout << "Input a choice(0, 1, 2): " ;
    cin >> choice ;
  }
}