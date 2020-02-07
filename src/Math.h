
/** \file Math.h
*   \brief Obsahuje deklaraci tridy Math.  
*/

#ifndef MATH_H_
#define MATH_H_

#include <algorithm>
#include "Circle.h"
#include <math.h>
#include "Point.h"
#include <SDL.h>
#include <string>
#include <SDL_opengl.h>
#include <vector>

using std::pair;
using std::string;
using std::vector;

typedef pair<Point, Point> line_t; //!<usecka dana svymi dvema krajnimi body
typedef vector<Point> polygon_t;   //!<polygon dany usporadanym seznamem svych krajnich bodu

/** \class Math
*   \brief Trida pro matematicke operace. Obsahuje definice pouzivanych matematickych konstant a staticke funkce pro ruzne matematicke vypocty. 
*/
class Math
{
public:

	static const float PI; //!<hodnota matematicke konstanty PI

	enum axes { X, Y };    //!<osy v rovine

	/** \param number Vstupni cislo.
	*   \return Retezcova reprezentace vstupniho cisla.
    */
	static string Int_To_String(int number);

	/** \param number Adresa, kam se ulozi vysledek.
	*   \return true, pokud se prevod podaril, jinak false.
    */
	static bool String_To_Int(string s_number, int* number);

	//! \param precision Pocet desetinnych mist vysledku.
	static string Float_To_String(GLfloat number, int precision);

	//! \brief Prevede uhel ve stupnich do intervalu <0-360).
	static GLfloat Degrees_To_Range(GLfloat degrees);

	//! \brief Prevede uhel v radianech do intervalu <0-2PI).
	static GLfloat Radians_To_Range(GLfloat radians); 

	//! \brief Prevede uhel z radianu na stupne.
	static GLfloat Radians_To_Degrees(GLfloat radians) { return radians * (180 / PI); } 

	//! \brief Prevede uhel ze stupnu na radiany.
	static GLfloat Degrees_To_Radians(GLfloat degrees) { return degrees * (PI / 180); }

	//! \brief Zaokrouhli cislo typu GLfloat na cele cislo.
	static GLfloat Round(GLfloat number);

	//! \brief Vrati vetsi z obou vstupnich hodnot.
	static GLfloat Greater(GLfloat value1, GLfloat value2);

	//! \brief Vrati mensi z obou vstupnich hodnot.
    static GLfloat Smaller(GLfloat value1, GLfloat value2);

	//! \brief Vrati hodnotu vstupniho celeho cisla po umazani jeho posledni cislice.
	static int Backspace_Number(int number) { return number / 10; }

	//! \brief Vrati nejmensi mocninu dvou, ktera je vetsi nebo rovna predanemu cislu. 
	static int Next_Power_Of_Two(int number);

	//! \brief Seradi vektor instanci tridy Point vzestupne podle zadaneho rozmeru.
	static void Sort_Points(vector<Point> & points, axes axis);

	//! \brief Seradi vektor rozliseni vzestupne.
	static void Sort_Resolutions(vector<SDL_Rect> & resolutions);

	//! \brief Zjisti vzdalenost mezi dvema body v rovine.
	static GLfloat Points_Distance(const Point & point1, const Point & point2);

	//! \brief Zjistuje pozici bodu v rovine po aplikaci danych transformaci posunuti a otoceni (ve stupnich).
	static Point Transform_Point(const Point & point, GLfloat x_translation, GLfloat y_translation, GLfloat rotation);

	//! \brief Zjistuje bod na kruznici se stredem v [0,0] s danym polomerem, ktery odpovida danemu uhlu ve stupnich.
	static Point Get_Circle_Angle_Point(GLfloat radius, GLfloat angle);

	//! \brief Zjistuje uhel udavajici smer od prvniho k druhemu bodu ve stupnich.
    static GLfloat Get_Direction(const Point & point1, const Point & point2);

	/** \brief Nasobeni dvou matic.
	*   \param matrix1 Prvni matice jako rows1*cols1 po sobe jdoucich hodnot, ktere se pouziji jako prvky matice rows1xcols1.
	*   \param rows1 Pocet radku prvni matice.
	*   \param cols1 Pocet sloupcu prvni matice, musi byt stejny jako pocet radku druhe matice.
	*   \param matrix2 Druha matice jako rows2*cols2 po sobe jdoucich hodnot, ktere se pouziji jako prvky matice rows2xcols2.
	*   \param rows2 Pocet radku druhe matice, musi byt stejny jako pocet sloupcu prvni matice.
	*   \param cols2 Pocet sloupcu druhe matice.
	*   \param result_matrix Sem se ulozi vysledna matice jako rows1*cols2 po sobe jdoucich hodnot.
    */
	static void Matrix_Multiplication(const GLfloat* matrix1, int rows1, int cols1, const GLfloat* matrix2, int rows2, int cols2, GLfloat* result_matrix);

	/** \brief Zjistuje pozici bodu v rovine po aplikaci dane transformacni matice.
	*   \param transformation_matrix Transformacni matice jako 16 po sobe jdoucich hodnot, ktere se pouziji jako prvky matice 4x4.
    */
	static Point Transform_Point(const Point & point, const GLfloat* transformation_matrix);

	/** \brief Zjistuje, zda maji dve usecky/primky nejake spolecne body.
	*   \param segments Urcuje, zda se maji brat v uvahu jen usecky (true) nebo cele primky urcene temito useckami (false).
	*   \param intersections Slouzi pro ulozeni pruseciku nebo bodu tvoricich usecku, ktera je spolecnou casti obou usecek, pokud ty lezi na teze primce. Pokud je NULL, konkretni pruseciky se nezjistuji. Pokud obe usecky lezi na teze primce a segments je true, bude obsahovat dva body urcujici jejich spolecnou cast nebo jeden bod, pokud se pouze dotykaji svymi konci nebo zadny bod, pokud se neprekryvaji. Pokud jsou primky totozne a segments je false, nebude obsahovat nic.
	*   \return true, pokud se usecky/primky protinaji nebo prekryvaji, jinak false.
    */
	static bool Lines_Intersection(const line_t & line1, const line_t & line2, bool segments, vector<Point> * intersections);

	/** \brief Zjistuje, zda ma usecka/primka nejaky spolecny bod s kruznici.
	*   \param segments Urcuje, zda se ma brat v uvahu jen usecka (true) nebo cela primka urcena touto useckou (false).
	*   \param intersections Slouzi pro ulozeni pruseciku. Pokud je NULL, konkretni pruseciky se nezjistuji.
	*   \return true, pokud existuje aspon jeden prusecik, jinak false.
    */
	static bool Line_Circle_Intersections(const line_t & line, const Circle & circle, bool segment, vector<Point> * intersections);

	/** \brief Zjistuje kolizi mezi dvemi kruhy.
	*   \return true, pokud kruhy koliduji, jinak false.
    */
	static bool Circles_Collision(const Circle & circle1, const Circle & circle2);

	/** \brief Zjistuje kolizi mezi useckou a polygonem.
	*   \param segments Urcuje, zda se ma brat v uvahu jen dana usecka (true) nebo cela primka urcena touto useckou (false).
	*   \param intersections Slouzi pro ulozeni pruseciku nebo bodu tvoricich usecku, ktera je spolecnou casti usecky a hrany polygonu, pokud ty lezi na teze primce. Pokud je NULL, konkretni pruseciky se nezjistuji. Pokud usecka lezi na teze primce s nekterou hranou polygonu a segments je true, bude obsahovat dva body urcujici jejich spolecnou cast nebo jeden bod, pokud se pouze dotykaji svymi konci nebo zadny bod, pokud se neprekryvaji. Pokud usecka lezi na teze primce s nekterou hranou polygonu a segments je false, nebude obsahovat nic.
	*   \return true, pokud usecka s polygonem kliduje, jinak false.
    */
	static bool Line_Polygon_Intersections(const line_t & line, const polygon_t & polygon, bool segment, vector<Point> * intersections);

	/** \brief Zjistuje, zda se dany bod nachazi uvnitr daneho polygonu.
	*   \return true, pokud se dany bod nachazi uvnitr daneho polygonu, jinak false.
    */
	static bool Point_In_Polygon(const polygon_t & polygon, const Point & point);

	/** \brief Zjistuje kolizi mezi polygonem a kruhem.
	*   \return true, pokud polygon s kruhem koliduji, jinak false.
    */
	static bool Polygon_Circle_Collision(const polygon_t & polygon, const Circle & circle);

	/** \brief Zjistuje kolizi mezi dvema polygony.
	*   \return true, pokud polygony koliduji, jinak false.
    */
	static bool Polygons_Collision(const polygon_t & polygon1, const polygon_t & polygon2);

private:

	/** \brief Porovnavaci funkce pro STL funkci sort, ktera se pouziva v metode Sort_Points().
	*   \return true, pokud je x-ova souradnice prvniho bodu mensi nez x-ova souradnice druheho bodu, jinak false.
    */
	static bool Points_X_Comparison (Point point1, Point point2) { return point1.x < point2.x; }

	/** \brief Porovnavaci funkce pro STL funkci sort, ktera se pouziva v metode Sort_Points().
	*   \return true, pokud je y-ova souradnice prvniho bodu mensi nez y-ova souradnice druheho bodu, jinak false.
    */
	static bool Points_Y_Comparison (Point point1, Point point2) { return point1.y < point2.y; }

	/** \brief Porovnavaci funkce pro STL funkci sort, ktera se pouziva v metode Sort_Resolutions().
	*   \return true, pokud je prvni rozliseni mensi nez druhe, jinak false.
    */
	static bool Resolutions_Comparison(const SDL_Rect & res1, const SDL_Rect & res2) { return res1.w * res1.h < res2.w * res2.h; }
	
};

#endif