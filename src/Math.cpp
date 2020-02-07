
/** \file Math.cpp
*   \brief Obsahuje definice metod a konstant tridy Math.
*/

#include "Math.h"
#include <sstream>
#include <utility>

using std::make_pair;
using std::sort;
using std::stringstream;

const float Math::PI = 3.14159265358979323846f;

Point Math::Transform_Point(const Point & point, const GLfloat* transformation_matrix)
{
	GLfloat matrix1[4] = { point.x, point.y, 0.0f, 1.0f };
	GLfloat* result_matrix = new GLfloat[1*4];
	Matrix_Multiplication(matrix1, 1, 4, transformation_matrix, 4, 4, result_matrix);
	Point result = Point(result_matrix[0], result_matrix[1]);
	delete result_matrix;
	return result;
}

void Math::Matrix_Multiplication(const GLfloat* matrix1, int rows1, int cols1, const GLfloat* matrix2, int rows2, int cols2, GLfloat* result_matrix)
{
	if (cols1 != rows2) return;

	for (int row = 0; row < rows1; row++)
	{
		for (int column = 0; column < cols2; column++)
		{
			result_matrix[row*cols2+column] = 0;
			for (int i = 0; i < cols1; i++)
			{
				result_matrix[row*cols2+column] += matrix1[row*cols1+i] * matrix2[i*cols2+column];
			}
		}
	}
}

GLfloat Math::Get_Direction(const Point & point1, const Point & point2)
{
	GLfloat x1 = point1.x;
	GLfloat y1 = point1.y;

	GLfloat x2 = point2.x;
	GLfloat y2 = point2.y;

	GLfloat angle = atan2(x2-x1, y1-y2);

	if (angle < 0) angle += 2*PI;

	return Radians_To_Degrees(angle);
}

Point Math::Transform_Point(const Point & point, GLfloat x_translation, GLfloat y_translation, GLfloat rotation)
{
	//ziskani odpovidajici transformacni matice
	glLoadIdentity();
	glTranslatef(x_translation, y_translation, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	GLfloat transformation_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, transformation_matrix); 
	glLoadIdentity();

	GLfloat point_matrix[4] = { point.x, point.y, 0.0f, 1.0f }; //rozsireni bodu na matici nasobitelnou s transformacni matici
	GLfloat result_matrix[4]; //vysledek maticoveho soucinu 

	Matrix_Multiplication(point_matrix, 1, 4, transformation_matrix, 4, 4, result_matrix);
	
	//nastaveni bodu z vysledne matice
	Point result;
	result.x = result_matrix[0];
	result.y = result_matrix[1];
	return result;
}

string Math::Float_To_String(GLfloat number, int precision)
{
	string s;
    stringstream out;
	out.setf(std::ios_base::fixed, std::ios_base::floatfield);
	out.precision(precision);
    out << number;
    s = out.str();

	return s;
}

string Math::Int_To_String(int number)
{
	string s;
    stringstream out;
    out << number;
    s = out.str();

	return s;
}

bool Math::String_To_Int(string s_number, int* number)
{
	int result = 0;
	bool negative = false;

	if (s_number[0] == '-')
	{
		negative = true;
		s_number.erase(s_number.begin());
	}
    
	for (unsigned int i = 0; i < s_number.size(); i++)
	{
		if (s_number[i] >= '0' && s_number[i] <= '9')
		{
			result *= 10;
			result += s_number[i] - '0';
		}
		else 
		{
			return false; //nepovoleny znak
		}
	}

	if (number != NULL)
	{
	    if (negative) result *= -1;
		*number = result;
	}
	
	return true;	
}

GLfloat Math::Round(GLfloat number) 
{
	GLfloat floor_number = floor(number);

	if (number - floor_number >= 0.5f)
	{
		return ceil(number);
	}
	else
	{
		return floor_number;
	}
}

GLfloat Math::Greater(GLfloat value1, GLfloat value2)
{
	if (value1 > value2)
		return value1;
	else
		return value2;
}

GLfloat Math::Smaller(GLfloat value1, GLfloat value2)
{
	if (value1 < value2)
		return value1;
	else
		return value2;
}

Point Math::Get_Circle_Angle_Point(GLfloat radius, GLfloat angle)
{
	return Point((sin(Degrees_To_Radians(angle))*radius), -(cos(Degrees_To_Radians(angle))*radius));
}

GLfloat Math::Degrees_To_Range(GLfloat degrees)
{
	if (degrees >= 360)
	{
		GLfloat num_circles = floor(degrees / 360);
		degrees -= num_circles * 360;
	}
	else if (degrees < 0.0f)
	{
		GLfloat num_circles = -floor(degrees / 360);
		degrees += num_circles * 360;
		degrees = 360 - degrees;
	}

	return degrees;
}

GLfloat Math::Radians_To_Range(GLfloat radians)
{
	if (radians >= 2*PI)
	{
		GLfloat num_circles = floor(radians / (2*PI));
		radians -= num_circles * 2*PI;
	}
	else if (radians < 0.0f)
	{
		GLfloat num_circles = -floor(radians / (2*PI));
		radians += num_circles * (2*PI);
		radians = (2*PI) - radians;
	}

	return radians;
}

int Math::Next_Power_Of_Two(int number)
{
	double logbase2 = log(double(number)) / log(double(2));
	return int(Math::Round(GLfloat(pow(2,ceil(logbase2)))));
}

void Math::Sort_Points(vector<Point> & points, axes axis)
{
	switch (axis)
	{
	case X:
		sort(points.begin(), points.end(), &Points_X_Comparison);
		break;
	case Y:
		sort(points.begin(), points.end(), &Points_Y_Comparison);
		break;
	default:
		break;
	}
}

void Math::Sort_Resolutions(vector<SDL_Rect> & resolutions)
{
	sort(resolutions.begin(), resolutions.end(), &Resolutions_Comparison);
}

GLfloat Math::Points_Distance(const Point & point1, const Point & point2)
{
	GLfloat x_diff = point1.x - point2.x;
	GLfloat y_diff = point1.y - point2.y;

	return sqrt(pow(x_diff, 2) + pow(y_diff, 2)); //Pythagorova veta
}

bool Math::Line_Circle_Intersections(const line_t & line, const Circle & circle, bool segment, vector<Point> * intersections)
{
    if (intersections != NULL) intersections->clear();

	vector<Point> possible_intersections;

	//Obecna rovnice kruznice:
	//x^2 + y^2 - 2*m*x - 2*n*y + p = 0

	GLfloat r = circle.Get_Radius();   //polomer kruznice
	GLfloat m = circle.Get_Center_X(); //x-ova souradnice stredu kruznice
	GLfloat n = circle.Get_Center_Y(); //y-ova souradnice stredu kruznice
	GLfloat p = pow(m, 2) + pow(n, 2) - pow(r, 2);

	//Smernicovy tvar primky:
    //y = k*x + q
	//k = (By - Ay) / (Bx - Ax)

	const Point & A = line.first;
	const Point & B = line.second;

	if (Round(A.x) == Round(B.x)) //svisla cara, nemuzeme vyjadrit smernicovym tvarem, dosadime x do rovnice kruznice a hledame y
	{
		//y^2 - 2*n*y + x^2 - 2m*x + p  = 0
		//obecny tvar kvadraticke rovnice:
		//a*x^2 + b*x + c = 0;

		GLfloat a = 1;
		GLfloat b = -2 * n;
		GLfloat c = pow(A.x, 2) - (2 * m * A.x) + p;
		GLfloat D = pow(b, 2) - (4 * a * c); //diskriminant kvadraticke rovnice

	    if (D >= 0.0f) //rovnice ma jedno nebo dve reseni, tecna nebo secna
		{
			possible_intersections.push_back(Point(A.x, (-b + sqrt(D)) / (2 * a))); //prvni reseni
			if (D != 0.0f) //ma i druhe reseni
			{
				possible_intersections.push_back(Point(A.x, (-b - sqrt(D)) / (2 * a)));
			}
			if (!segment) //zajima nas primka, ne usecka, konec
			{
				if (intersections) *intersections = possible_intersections;
				return true;
			}
		}
	}
	else //cara neni svisla, muzeme vyjadrit smernicovym tvarem
	{
		GLfloat k = (B.y - A.y) / (B.x - A.x);
		GLfloat q = A.y - (k * A.x);
		
		//x^2 + y^2 - 2*m*x - 2*n*y + p  = 0
		//Dosadime k*x + q za y
		//x^2 + k^2*x^2 + 2*k*x*q + q^2 - 2*m*x - 2*n*k*x - 2*n*q + p = 0
		//(k^2 + 1)*x^2 + (2*k*q - 2*m - 2*n*k)x + (q^2- 2*n*q + p) = 0
		//obecny tvar kvadraticke rovnice:
		//a*x^2 + b*x + c = 0;
		GLfloat a = pow(k, 2) + 1;
		GLfloat b = (2 * k * q) - (2 * m) - (2 * n * k);
		GLfloat c = pow(q, 2) - (2 * n * q) + p;
		GLfloat D = pow(b, 2) - (4 * a * c);
	
		if (D >= 0.0f) //rovnice ma jedno nebo dve reseni, tecna nebo secna
		{
			GLfloat x = (-b + sqrt(D)) / (2 * a); //reseni
			GLfloat y = (k * x) + q;
			possible_intersections.push_back(Point(x, y));

			if (D != 0.0f) //ma i druhe reseni
			{
				x = (-b - sqrt(D)) / (2 * a);
			    y = (k * x) + q;
			    possible_intersections.push_back(Point(x, y));
			}

			if (!segment) //zajima nas primka, ne usecka, konec
			{
				if (intersections) *intersections = possible_intersections;
				return true;
			}
		}
	}

	bool in_segment = false; //kdyz jsme dosli az sem, zajima nas prusecik s useckou, zjistujeme, zda zjistene pruseciky lezi mezi krajnimi body teto usecky

	for (unsigned int i = 0; i < possible_intersections.size(); i++)
	{
		if (Round(A.x) == Round(B.x)) //x jsou stejna, porovnavame podle y
		{
			GLfloat AB_min_y =  Smaller(A.y, B.y);
			GLfloat AB_max_y =  Greater(A.y, B.y);
			if (possible_intersections[i].y >= AB_min_y && possible_intersections[i].y <= AB_max_y) //prusecik lezi na usecce
			{
				if (intersections) intersections->push_back(possible_intersections[i]);
				in_segment = true;
			}
		}
		else //porovnavame podle x
		{
			GLfloat ABminX =  Smaller(A.x, B.x);
			GLfloat ABmaxX =  Greater(A.x, B.x);
			if (possible_intersections[i].x >= ABminX && possible_intersections[i].x <= ABmaxX) //prusecik lezi na usecce
			{
				if (intersections) intersections->push_back(possible_intersections[i]);
				in_segment = true;
			}
		}
	}

	return in_segment;
}

bool Math::Lines_Intersection(const line_t & line1, const line_t & line2, bool segments, vector<Point> * intersections)
{
	if (intersections != NULL) intersections->clear();

	const Point & A = line1.first;
	const Point & B = line1.second;
	const Point & C = line2.first;
	const Point & D = line2.second;
	
    vector<Point> possible_intersections;
	bool same = false; //jesli usecky lezi na spolecne primce

	//Smernicovy tvar primky:
    //y = k*x + q
	//k = (By - Ay) / (Bx - Ax)

	if (A.x == B.x && C.x == D.x) //obe cary svisle
	{
		if (A.x == C.x) same = true;
	}
	else if (A.y == B.y && C.y == D.y) //obe cary vodorovne
	{
		if (A.y == C.y) same = true;
	}
	else if (A.x == B.x) //AB svisla, CD nesvisla
	{
	    GLfloat k = (D.y - C.y) / (D.x - C.x);
		GLfloat q = C.y - (k * C.x);
		possible_intersections.push_back(Point(A.x, (k * A.x) + q));
	}
	else if (C.x == D.x) //CD svisla, AB nesvisla
	{
		GLfloat k = (B.y - A.y) / (B.x - A.x);
		GLfloat q = A.y - (k * A.x);
		possible_intersections.push_back(Point(C.x, (k * C.x) + q));
	}
	else if (A.y == B.y) //AB vodorovna, CD ani vodorovna ani svisla
	{
		GLfloat k = (C.y - D.y) / (C.x - D.x);
		GLfloat q = C.y - (k * C.x);
		possible_intersections.push_back(Point((A.y - q) / k, A.y));
	}
	else if (C.y == D.y) //CD vodorovna, AB ani vodorovna ani svisla
	{
		GLfloat k = (A.y - B.y) / (A.x - B.x);
		GLfloat q = A.y - (k * A.x);
		possible_intersections.push_back(Point((C.y - q) / k, C.y));
	}
	else //ani jedna svisla nebo vodorovna
	{
		GLfloat k1 = (B.y - A.y) / (B.x - A.x);
		GLfloat q1 = A.y - (k1 * A.x);
		GLfloat k2 = (D.y - C.y) / (D.x - C.x);
		GLfloat q2 = C.y - (k2 * C.x);
		
        if (k1 == k2) //rovnobezky
		{
			if (q1 == q2) //totozne primky
			{
				same = true;
			}
		}
		else //nejsou rovnobezky
		{
			//k1*x + q1 = k2*x + q2
			//k1*x - k2*x = q2 - q1
			GLfloat x = (q2 - q1) / (k1 - k2);
		    GLfloat y = (k2 * x) + q2;
			possible_intersections.push_back(Point(x, y));
		}
	}

	if (same) //totozne primky
	{
		if (!segments) return true; //zajimaji nas primky, ne usecky, konec

		GLfloat CD_max_x = Greater(C.x, D.x);
	    GLfloat CD_min_x = Smaller(C.x, D.x);
	    GLfloat AB_max_x = Greater(A.x, B.x);
        GLfloat AB_min_x = Smaller(A.x, B.x);
		GLfloat CD_max_y = Greater(C.y, D.y);
	    GLfloat CD_min_y = Smaller(C.y, D.y);
	    GLfloat AB_max_y = Greater(A.y, B.y);
        GLfloat AB_min_y = Smaller(A.y, B.y);

	    //pokud maji usecky spolecny segment
        if ((AB_min_x <= CD_max_x && CD_min_x <= AB_max_x) && (AB_min_y <= CD_max_y && CD_min_y <= AB_max_y))
	    {	
		    if (intersections) //zajima nas spolecna usecka nebo bod
		    {
				Point points[4] = { A, B, C, D };
				vector<Point> sorted_points(points, points + 4 );
				if (A.x == B.x) //x jsou stejna, radime podle y
				{
					Sort_Points(sorted_points, Y);
				}
				else //radime podle x
				{
					Sort_Points(sorted_points, X);
				}

				intersections->push_back(sorted_points[1]);

			    if (sorted_points[1].x != sorted_points[2].x || sorted_points[1].y != sorted_points[2].y)
				{
					intersections->push_back(sorted_points[2]);
				}
		    }
	
		    return true;
	    }
	}
	else if (possible_intersections.size() != 0) //primky nejsou totozne, maji jeden prusecik
	{
		if (!segments) //zajimaji nas primky, ne usecky, konec
		{
			if (intersections) *intersections = possible_intersections; //zajima nas spolecny bod
			return true;
		}

		GLfloat x = Math::Round(possible_intersections[0].x);
		GLfloat y = Math::Round(possible_intersections[0].y);

		GLfloat CD_max_x = Math::Round(Greater(C.x, D.x));
	    GLfloat CD_min_x = Math::Round(Smaller(C.x, D.x));
	    GLfloat AB_max_x = Math::Round(Greater(A.x, B.x));
        GLfloat AB_min_x = Math::Round(Smaller(A.x, B.x));
		GLfloat CD_max_y = Math::Round(Greater(C.y, D.y));
	    GLfloat CD_min_y = Math::Round(Smaller(C.y, D.y));
	    GLfloat AB_max_y = Math::Round(Greater(A.y, B.y));
        GLfloat AB_min_y = Math::Round(Smaller(A.y, B.y));

		//pokud lezi prusecik primek v mezich usecek
        if (x >= AB_min_x && x <= AB_max_x && x >= CD_min_x && x <= CD_max_x && y >= AB_min_y && y <= AB_max_y && y >= CD_min_y && y <= CD_max_y)
	    {
			if (intersections) *intersections = possible_intersections; //zajima nas spolecny bod
			    return true;
		}
	}

	return false;
}

bool Math::Circles_Collision(const Circle & circle1, const Circle & circle2)
{
	GLfloat distance = Points_Distance(circle1.Get_Center(), circle2.Get_Center()); //vzdalenost jejich stredu

	if (distance <= circle1.Get_Radius() + circle2.Get_Radius())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Math::Line_Polygon_Intersections(const line_t & line, const polygon_t & polygon, bool segment, vector<Point> * intersections)
{
	if (intersections != NULL) intersections->clear();

	for (vector<Point>::const_iterator it = polygon.begin(); it < polygon.end(); it++)
	{
		line_t polygon_line; //usecka mezi dvema vedlejsimi body polygonu
		if (it == polygon.end()-1) //posledni bod polygonu, musi se udelat usecka napojena na prvni bod polygonu 
		{
			polygon_line = make_pair(Point(it->x, it->y), Point(polygon.begin()->x, polygon.begin()->y));
		}
		else //usecka mezi dvema sousednimi body polygonu
		{
		    polygon_line = make_pair(Point(it->x, it->y), Point((it+1)->x, (it+1)->y));
		}

		if (intersections != NULL)
		{
			vector<Point> inters;
		    Lines_Intersection(line, polygon_line, segment, &inters);
			intersections->insert(intersections->end(), inters.begin(), inters.end());
		}
		else
		{
			if (Lines_Intersection(line, polygon_line, segment, NULL))
			{
			    return true;
			}
		}
	}

	if (intersections != NULL && !intersections->empty())
	{
		return true;
	}
	else
	{
	    return false;
	}
}

bool Math::Point_In_Polygon(const polygon_t & polygon, const Point & point)
{
	line_t line = make_pair(point, Point(point.x + (5000 * 2), point.y)); //vodorovna usecka z daneho bodu doprava az za konec obrazovky

	int count = 0; //pocet pruseciku usecky s polygonem

	for (vector<Point>::const_iterator it = polygon.begin(); it < polygon.end(); it++)
	{
		line_t polygon_line; //usecka mezi dvema vedlejsimi body polygonu
		if (it == polygon.end()-1) //posledni bod polygonu, musi se udelat usecka napojena na prvni bod polygonu 
		{
			polygon_line = make_pair(Point(it->x, it->y), Point(polygon.begin()->x, polygon.begin()->y));
		}
		else //usecka mezi dvema sousednimi body polygonu
		{
		    polygon_line = make_pair(Point(it->x, it->y), Point((it+1)->x, (it+1)->y));
		}
		if (Lines_Intersection(polygon_line, line, true, NULL))
		{
			count++;
		}
	}

	if (count % 2) //lichy pocet pruseciku, bod je vevnitr
	{
		return true;
	}
	else //sudy pocet pruseciku, bod neni vevnitr
	{
		return false;
	}
}

bool Math::Polygon_Circle_Collision(const polygon_t & polygon, const Circle & circle)
{
	for (vector<Point>::const_iterator it = polygon.begin(); it < polygon.end(); it++)
	{
		line_t polygon_line; //usecka mezi dvema vedlejsimi body polygonu
		if (it == polygon.end()-1) //posledni bod polygonu, musi se udelat usecka napojena na prvni bod polygonu 
		{
			polygon_line = make_pair(Point(it->x, it->y), Point(polygon.begin()->x, polygon.begin()->y));
		}
		else //usecka mezi dvema sousednimi body polygonu
		{
		    polygon_line = make_pair(Point(it->x, it->y), Point((it+1)->x, (it+1)->y));
		}
		if (Line_Circle_Intersections(polygon_line, circle, true, NULL)) 
		{
			return true;
		}
	}

	return false;
}

bool Math::Polygons_Collision(const polygon_t & polygon1, const polygon_t & polygon2)
{
	for (vector<Point>::const_iterator it1 = polygon1.begin(); it1 < polygon1.end(); it1++)
	{
		line_t polygon_line1; //usecka mezi dvema vedlejsimi body polygonu
		if (it1 == polygon1.end()-1) //posledni bod polygonu, musi se udelat usecka napojena na prvni bod polygonu 
		{
			polygon_line1 = make_pair(Point(it1->x, it1->y), Point(polygon1.begin()->x, polygon1.begin()->y));
		}
		else //usecka mezi dvema sousednimi body polygonu
		{
		    polygon_line1 = make_pair(Point(it1->x, it1->y), Point((it1+1)->x, (it1+1)->y));
		}
		
	    for (vector<Point>::const_iterator it2 = polygon2.begin(); it2 < polygon2.end(); it2++)
		{
		    line_t polygon_line2; //usecka mezi dvema vedlejsimi body polygonu
		    if (it2 == polygon2.end()-1) //posledni bod polygonu, musi se udelat usecka napojena na prvni bod polygonu 
		    {
			    polygon_line2 = make_pair(Point(it2->x, it2->y), Point(polygon2.begin()->x, polygon2.begin()->y));
		    }
		    else //usecka mezi dvema sousednimi body polygonu
		    {
		        polygon_line2 = make_pair(Point(it2->x, it2->y), Point((it2+1)->x, (it2+1)->y));
		    }

			if (Lines_Intersection(polygon_line1, polygon_line2, true, NULL)) 
			{
				return true;
			}
		}
	}

	return false;
}