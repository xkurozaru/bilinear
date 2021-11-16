#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

#define PI 3.14159265359

class Image
{
public:
	vector<vector<unsigned char>> r;
	vector<vector<unsigned char>> g;
	vector<vector<unsigned char>> b;

	Image(int x_size=0, int y_size=0)
	{
		r.resize(y_size);
		g.resize(y_size);
		b.resize(y_size);
		for (int y = 0; y < y_size; y++)
		{
			r[y].resize(x_size);
			g[y].resize(x_size);
			b[y].resize(x_size);
		}

		for (int y = 0; y < y_size; y++)
		{
			for (int x = 0; x < x_size; x++)
			{
				r[y][x] = 0;
				g[y][x] = 0;
				b[y][x] = 0;
			}
		}
	}

	void read_image(int x_size, int y_size, string filename)
	{
		vector<vector<unsigned char>> r(y_size, vector<unsigned char>(x_size));
		vector<vector<unsigned char>> g(y_size, vector<unsigned char>(x_size));
		vector<vector<unsigned char>> b(y_size, vector<unsigned char>(x_size));

		ifstream file_in;
		file_in.open(filename, std::ios::in | std::ios::binary);
		if (!file_in.is_open())
		{
			cout << "file is not opened.";
			exit(-1);
		}

		unsigned char tmp;
		for (int y = 0; y < y_size; y++)
		{
			for (int x = 0; x < x_size; x++)
			{
				file_in.read((char*)&tmp, sizeof(unsigned char));
				r[y][x] = unsigned char(tmp);
				file_in.read((char*)&tmp, sizeof(unsigned char));
				g[y][x] = unsigned char(tmp);
				file_in.read((char*)&tmp, sizeof(unsigned char));
				b[y][x] = unsigned char(tmp);
			}
		}

		this->r = r;
		this->g = g;
		this->b = b;
		file_in.close();
	}

	void print_image(string filename)
	{
		ofstream file_out;
		file_out.open(filename, std::ios::out | std::ios::binary);
		if (!file_out.is_open())
		{
			cout << "file is not opened.";
			exit(-1);
		}

		int x_size = this->b[0].size();
		int y_size = this->b.size();
		for (int y = 0; y < y_size; y++)
		{
			for (int x = 0; x < x_size; x++)
			{
				file_out << this->r[y][x];
				file_out << this->g[y][x];
				file_out << this->b[y][x];
			}
		}

		file_out.close();
	}

	int xsize()
	{
		return this->r[0].size();
	}
	int ysize()
	{
		return this->r.size();
	}
};

void rotate(Image *InImg, Image *RotatedImg, double angle)
{
	double theta = -angle * PI / 180.0;
	int xd, yd;
	int x_size = InImg->xsize();
	int y_size = InImg->ysize();

	for (int y = 0; y < y_size; y++)
	{
		for (int x = 0; x < x_size; x++)
		{
			xd = (x - x_size / 2.0) * cos(theta) + (y - y_size / 2.0) * sin(theta) + x_size / 2.0;
			yd = -(x - x_size / 2.0) * sin(theta) + (y - y_size / 2.0) * cos(theta) + y_size / 2.0;
			
			if (xd>=0 && xd<x_size && yd>=0 && yd<y_size)
			{
				RotatedImg->r[y][x] = InImg->r[yd][xd];
				RotatedImg->g[y][x] = InImg->g[yd][xd];
				RotatedImg->b[y][x] = InImg->b[yd][xd];
			}
		}
	}
}

void nearest(Image* InImg, Image* NearestImg, double rate)
{
	int ix, iy;
	double xa, ya;
	int x_size = InImg->xsize() * rate;
	int y_size = InImg->ysize() * rate;
	
	for (int y = 0; y < y_size; y++)
	{
		for (int x = 0; x < x_size; x++)
		{
			xa = x / rate;
			ya = y / rate;
			ix = int(xa);
			iy = int(ya);

			if (ix + 1 < InImg->xsize() && iy + 1 < InImg->ysize())
			{
				NearestImg->r[y][x] = InImg->r[iy][ix];
				NearestImg->g[y][x] = InImg->g[iy][ix];
				NearestImg->b[y][x] = InImg->b[iy][ix];
			}
		}
	}
}

void bilinear(Image* InImg, Image* BilinearImg, double rate)
{
	int xd, yd, ix, iy;
	double xa, ya, dx, dy;
	int x_size = InImg->xsize() * rate;
	int y_size = InImg->ysize() * rate;

	for (int y = 0; y < y_size; y++)
	{
		for (int x = 0; x < x_size; x++)
		{
			xa = x / rate;
			ya = y / rate;
			ix = int(xa);
			iy = int(ya);
			dx = xa - ix;
			dy = ya - iy;

			if (ix + 1 < InImg->xsize() && iy + 1 < InImg->ysize())
			{
				BilinearImg->r[y][x] = (1 - dx) * (1 - dy) * InImg->r[iy][ix] + dx * (1 - dy) * InImg->r[iy][ix + 1] + (1 - dx) * dy * InImg->r[iy + 1][ix] + dx * dy * InImg->r[iy + 1][ix + 1];
				BilinearImg->g[y][x] = (1 - dx) * (1 - dy) * InImg->g[iy][ix] + dx * (1 - dy) * InImg->g[iy][ix + 1] + (1 - dx) * dy * InImg->g[iy + 1][ix] + dx * dy * InImg->g[iy + 1][ix + 1];
				BilinearImg->b[y][x] = (1 - dx) * (1 - dy) * InImg->b[iy][ix] + dx * (1 - dy) * InImg->b[iy][ix + 1] + (1 - dx) * dy * InImg->b[iy + 1][ix] + dx * dy * InImg->b[iy + 1][ix + 1];
			}
		}
	}
}

void menu1(Image* InImg);
void menu2(Image* InImg, Image* RotatedImg);
void menu3(Image* InImg, Image* NearestImg);
void menu4(Image* InImg, Image* BilinearImg);
void menu5(Image* InImg);

int main(void)
{
	string select;
	Image InImg, OutImg;
	cout << "Select key number." << endl;
	while (1)
	{
		cout << "-----------------------------------" << endl;
		cout << "  1.Read File " << endl;
		cout << "  2.Rotate " << endl;
		cout << "  3.Nearest neighbor " << endl;
		cout << "  4.Bi-linear interpolation " << endl;
		cout << "  5.Save File " << endl;
		cout << "  6.Exit " << endl;
		cout << "-----------------------------------" << endl;
		cout << "select >>";
		cin >> select;
		system("cls");

		if (select == "1")
		{
			menu1(&InImg);
			system("cls");
			cout << "-----------------------" << endl;
			cout << "| ImageSize(" << InImg.xsize() << "," << InImg.ysize() << ") |" << endl;
			cout << "-----------------------" << endl;
			cout << "Read File Complete." << endl;
		}
		else if (select == "2")
		{
			menu2(&InImg, &OutImg);
			InImg = OutImg;
			system("cls");
			cout << "-----------------------" << endl;
			cout << "| ImageSize(" << InImg.xsize() << "," << InImg.ysize() << ") |" << endl;
			cout << "-----------------------" << endl;
			cout << "Rotate Complete." << endl;
		}
		else if (select == "3")
		{
			menu3(&InImg, &OutImg);
			InImg = OutImg;
			system("cls");
			cout << "-----------------------" << endl;
			cout << "| ImageSize(" << InImg.xsize() << "," << InImg.ysize() << ") |" << endl;
			cout << "-----------------------" << endl;
			cout << "Nearest neighbor Complete." << endl;
		}
		else if (select == "4")
		{
			menu4(&InImg, &OutImg);
			InImg = OutImg;
			system("cls");
			cout << "-----------------------" << endl;
			cout << "| ImageSize(" << InImg.xsize() << "," << InImg.ysize() << ") |" << endl;
			cout << "-----------------------" << endl;
			cout << "Bi-linear interpolation Complete." << endl;
		}
		else if (select == "5")
		{
			menu5(&InImg);
			system("cls");
			cout << "-----------------------" << endl;
			cout << "| ImageSize(" << InImg.xsize() << "," << InImg.ysize() << ") |" << endl;
			cout << "-----------------------" << endl;
			cout << "Save File Complete." << endl;
		}
		else if (select == "6")
		{
			cout << "Close This Application." << endl;
			exit(0);
		}
		else
		{
			cout << "Input Incorrect." << endl;
		}
	}
}

void menu1(Image* InImg)
{
	cout << "-------------------" << endl;
	cout << " Read File " << endl;
	cout << "-------------------" << endl;
	cout << "Read file name >>";
	string filename;
	cin >> filename;
	int x_size, y_size;
	cout << "size of x >>";
	cin >> x_size;
	cout << "size of y >>";
	cin >> y_size;
	InImg->read_image(x_size, y_size, filename);
}

void menu2(Image* InImg, Image* RotatedImg)
{
	double angle;
	cout << "-------------------" << endl;
	cout << " Rotate " << endl;
	cout << "-------------------" << endl;
	cout << "rotate angle >>";
	cin >> angle;
	int x_size = InImg->xsize();
	int y_size = InImg->ysize();
	*RotatedImg = Image(x_size, y_size);
	rotate(InImg, RotatedImg, angle);
}

void menu3(Image* InImg, Image* NearestImg)
{
	double rate;
	cout << "-------------------" << endl;
	cout << " Nearest neighbor " << endl;
	cout << "-------------------" << endl;
	cout << "expansion rate >>";
	cin >> rate;
	int x_size = InImg->xsize() * rate;
	int y_size = InImg->ysize() * rate;
	*NearestImg = Image(x_size, y_size);
	nearest(InImg, NearestImg, rate);
}

void menu4(Image* InImg, Image* BilinearImg)
{
	double rate;
	cout << "-------------------" << endl;
	cout << " Bi-linear interpolation " << endl;
	cout << "-------------------" << endl;
	cout << "expansion rate >>";
	cin >> rate;
	int x_size = InImg->xsize() * rate;
	int y_size = InImg->ysize() * rate;
	*BilinearImg = Image(x_size, y_size);
	bilinear(InImg, BilinearImg, rate);
}

void menu5(Image* InImg)
{
	cout << "-------------------" << endl;
	cout << " Save File " << endl;
	cout << "-------------------" << endl;
	cout << "Save file name >>";
	string filename;
	cin >> filename;
	InImg->print_image(filename);
}