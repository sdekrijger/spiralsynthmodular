#ifndef __STRING_H__
#define __STRING_H__
#ifdef __cplusplus

#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

/*
 * Cette classe permet de stocker des cha�nes de caract�res simplement.
 * Elle est une base essentielle et permet d'effectuer �norm�ment d'op�rations:
 *   - set fonctionne comme sprintf, et r�assigne la cha�ne.
 *   - to_charp() transforme la cha�ne en pointeur.
 *   - to_charp(from) extrait la cha�ne depuis le caract�re num�ro from
 *   - to_charp(from, to) extrait la cha�ne depuis le caract�re num�ro from
 *                        jusqu'au caract�re num�ro to.
 *        ATTENTION: les fonctions to_charp renvoie un pointeur sur un tableau
 *                   statique, et ce tableau sera modifi� au prochain
 *                   appel � to_charp ou a set.
 *   - to_int transforme la cha�ne en int.
 *   - to_double transforme la cha�ne en double
 *   - to_sqldate effectue la conversion suivante: DD/MM/YYYY ==> YYYYMMDD
 *   - to_sqltime effectue la conversion suivante: h:m ==> h * 60 + m
 *   - from_sqldate et from_sqltime effectue les conversions inverses.
 *   - is_date, is_number, is_float, is_time renvoient des bool�ens de test.
 *   - strlen renvoie la longueur de la chaine
 *   - strchr(c) renvoie la position du caract�re c
 *   - strchr(c, p) recherche le caract�re c � partir de la position p
 *   - strrchr(c) renvoie la position du caract�re c, en recherchant depuis la droite.
 *   - strstr(s) renvoie la position de la chaine s
 *        NOTE: les fonctions de recherche renvoient un r�sultat n�gatif si pas trouv�.
 *   - strchrcnt(c) compte le nombre d'occurences du caract�re c.
 *
 *   Les op�rateurs !=, ==, <=, >=, < et > permettent de comparer deux cha�nes, en mode
 *   'case sensitive'. L'operateur [] renvoie un caract�re de la cha�ne.
 *
 *   Les op�rateurs << et >> vers un ostream ou depuis un istream sont support�s.
 */
/*
class String {
  public:
      String(const String &);
      String(const char * = NULL);
      String(char);
      String(int);
      String(double);
      ~String();
      char * set(char *, ...);
      char * to_charp(size_t = 0, ssize_t = -1) const;
      int to_int() const;
      string to_string() const;
      double to_double() const;
      String to_sqldate() const;
      String to_sqltime() const;
      String from_sqldate() const;
      String from_sqltime() const;
      bool is_date() const;
      bool is_number() const;
      bool is_float() const;
      bool is_time() const;
      size_t strlen() const;
      ssize_t strchr(char, size_t = 0) const;
      ssize_t strrchr(char) const;
      ssize_t strstr(const String &) const;
      int strchrcnt(char) const;
      String & operator=(const String &);
      String operator+(const String &) const;
      String & operator+=(const String &);
      bool operator!=(const String &) const;
      bool operator==(const String &) const;
      bool operator<=(const String &) const;
      bool operator>=(const String &) const;
      bool operator<(const String &) const;
      bool operator>(const String &) const;
      char operator[](size_t i) const;
  private:
    static char t[BUFSIZ];
    char * str;
};

ostream & operator<<(ostream &, const String &);
istream & operator>>(istream &, String &);
*/
#else
#error This only works with a C++ compiler
#endif
#endif
