#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
int map[500][500];
class Rasterizer {
public:
	//绘制直线
	static std::vector<std::vector<long long>> drawLine(std::vector<long long>st, std::vector<long long>ed) {
		if (st[0] == ed[0] && st[1] == ed[1]) {
			return {};
		}
		std::vector<std::vector<long long>> ans;
		double deltaX = abs(st[0] - ed[0]);
		double deltaY = abs(st[1] - ed[1]);
		bool scanYAxis = (deltaY > deltaX);
		bool descending = false;
		std::vector<long long>rSt, rEd;
		if (scanYAxis) {
			if (ed[1] > st[1]) {
				rSt = st;
				rEd = ed;
			}
			else {
				rSt = ed;
				rEd = st;
			}
			if (rEd[0] - rSt[0] < 0) {
				descending = true;
			}
		}
		else {
			if (ed[0] > st[0]) {
				rSt = st;
				rEd = ed;
			}
			else {
				rSt = ed;
				rEd = st;
			}
			if (rEd[1] - rSt[1] < 0) {
				descending = true;
			}
		}
		//Bresenham 直线算法
		if (!scanYAxis) {
			int dx = (int)round(rSt[0]), dy = (int)round(rSt[1]);
			double slope = (double)(rEd[1] - rSt[1]) / (double)(rEd[0] - rSt[0]);
			double e = -0.5;
			if (descending) {
				e = 0.5;
			}
			for (int i = dx; i <= (int)round(rEd[0]); i++) {
				ans.push_back({ dx,dy });
				dx++;
				e += slope;
				if (!descending) {
					if (e >= 0) {
						dy++;
						e = e - 1;
					}
				}
				else {
					if (e <= 00) {
						dy--;
						e = e + 1;
					}
				}
			}
		}
		else {
			int dx = (int)round(rSt[1]), dy = (int)round(rSt[0]);
			double slope = (double)(rEd[0] - rSt[0]) / (double)(rEd[1] - rSt[1]);
			double e = -0.5;
			if (descending) {
				e = 0.5;
			}
			for (int i = dx; i <= (int)round(rEd[1]); i++) {
				ans.push_back({ dy,dx });
				dx++;
				e += slope;
				if (!descending) {
					if (e >= 0) {
						dy++;
						e = e - 1;
					}
				}
				else {
					if (e <= 00) {
						dy--;
						e = e + 1;
					}
				}
			}
		}
		return ans;
	}
	//绘制矩形
	static std::vector<std::vector<long long>> drawRect(std::vector<std::vector<long long>> pt) {
		std::vector<std::vector<long long>> ret;
		memset(map, 0, sizeof(map));
		for (int i = 0; i < 4; i++) {
			std::vector<std::vector<long long>> ans = drawLine(pt[i], pt[(i + 1) % 4]);
			for (int j = 0; j < ans.size(); j++) {
				if (ans[j][0] >= 0 && ans[j][0] < 500 && ans[j][1] >= 0 && ans[j][1] < 500)
					map[ans[j][0]][ans[j][1]] = 1;
				ret.push_back({ ans[j][0],ans[j][1] });
			}
		}
		std::queue<long long> x;
		std::queue<long long>y;
		std::vector<std::vector<long long>> r = drawLine(pt[1], pt[3]);
		for (int i = 1; i < r.size() - 1; i++) {

			if (r[i][0] >= 0 && r[i][0] < 500 && r[i][1] >= 0 && r[i][1] < 500) {
				if (map[r[i][0]][r[i][1]] == 1) {
					continue;
				}
				x.push(r[i][0]);
				y.push(r[i][1]);
				map[r[i][0]][r[i][1]] = 1;
			}

		}
		while (!x.empty()) {
			long long tx = x.front(); x.pop();
			long long ty = y.front(); y.pop();
			//std::cout << tx << "," << ty << std::endl;
			ret.push_back({ tx,ty });
			if (tx - 1 >= 0 && tx - 1 < 500) {
				if (!map[tx - 1][ty]) {
					x.push(tx - 1);
					y.push(ty);
					map[tx - 1][ty] = 1;
				}

			}
			if (tx + 1 >= 0 && tx + 1 < 500) {
				if (!map[tx + 1][ty]) {
					x.push(tx + 1);
					y.push(ty);
					map[tx + 1][ty] = 1;
				}
			}
			if (ty + 1 >= 0 && ty + 1 < 500) {
				if (!map[tx][ty + 1]) {
					x.push(tx);
					y.push(ty + 1);
					map[tx][ty + 1] = 1;
				}
			}
			if (ty - 1 >= 0 && ty - 1 < 500) {
				if (!map[tx][ty - 1]) {
					x.push(tx);
					y.push(ty - 1);
					map[tx][ty - 1] = 1;
				}
			}
		}
		return ret;
	}
	//绘制粗直线
	static std::vector<std::vector<long long>> drawLineW(std::vector<long long>st, std::vector<long long>ed, int lineWidth) {
		long long A = st[1] - ed[1];
		long long B = -st[0] + ed[0];
		long long x1 = st[0] + A * lineWidth / 2.0 / sqrt(A * A + B * B);
		long long x2 = st[0] - A * lineWidth / 2.0 / sqrt(A * A + B * B);
		long long y1 = st[1] + B * lineWidth / 2 / sqrt(A * A + B * B);
		long long y2 = st[1] - B * lineWidth / 2 / sqrt(A * A + B * B);
		long long x3 = ed[0] + A * lineWidth / 2.0 / sqrt(A * A + B * B);
		long long x4 = ed[0] - A * lineWidth / 2.0 / sqrt(A * A + B * B);
		long long y3 = ed[1] + B * lineWidth / 2 / sqrt(A * A + B * B);
		long long y4 = ed[1] - B * lineWidth / 2 / sqrt(A * A + B * B);
		std::vector<std::vector<long long>> ans = drawRect({ {x1,y1},{x2,y2},{x4,y4},{x3,y3} });
		
		cv::Mat cvMat = cv::Mat(500, 500, CV_8UC3); 
		for (int i = 0; i < ans.size(); i++) {
			if (ans[i][0] < 0 || ans[i][1] < 0) {
				continue;
			}
			std::cout << ans[i][0] << "," << ans[i][1] << ";";
			cvMat.at<cv::Vec3b>(ans[i][0], ans[i][1]) = cv::Vec3b(255, 255, 255);
		}
		cv::imshow("Line", cvMat);
		return ans;
	}
	//绘制圆 (Brehensam 算法)
	static std::vector<std::vector<long long>> drawCircle(long long centerX,long long centerY,long long radius) {
		if (radius <= 0) {
			std::cout << "给定参数非法" << std::endl;
			return {};
		}
		std::vector<std::vector<long long>> ans;
		long long x0 = centerX;
		long long y0 = centerY + radius;
		HANDLE hout = GetStdHandle(-11);
		//(x+1)^2+y^2+(x+1)^2+(y-1)^2 = 2x^2+4x+2+2y^2-2y+1
		//x=0,y=R d=3-2R
		long long d = 3 - 2 * radius;
		ans.push_back({ x0,y0 });
		while (x0 - centerX <= y0 - centerY) {
			long long dx = x0 - centerX, dy = y0 - centerY;

			ans.push_back({ dx + centerX,dy + centerY });
			ans.push_back({ -dx + centerX,dy + centerY });
			ans.push_back({ dx + centerX,-dy + centerY });
			ans.push_back({ -dx + centerX,-dy + centerY });
			ans.push_back({ dy + centerX,dx + centerY });
			ans.push_back({ -dy + centerX,dx + centerY });
			ans.push_back({ dy + centerX,-dx + centerY });
			ans.push_back({ -dy + centerX,-dx + centerY });

			if (d < 0) { //如果delta<0,选择Hi(xi+1,yi)
				//(x+2)^2+y^2+(x+2)^2+(y-1)^2 = 2x^2+8x+8+2y^2-2y+1
				d += 4 * (x0 - centerX ) + 6;
				x0++;
			}
			else {  //否则选择Li
				//(x+2)^2+(y-1)^2+(x+2)^2+(y-2)^2 = 2x^2+8x+8+2y^2-6y+5 (+13)
				d += 4 * (x0 - centerX - (y0 - centerY)) + 10;
				x0++;
				y0--;
			}
		}
		cv::Mat cvMat = cv::Mat(500, 500, CV_8UC3);
		for (long long i = 0; i < ans.size(); i++) {
			COORD t;
			t.X = ans[i][0];
			t.Y = ans[i][1];
			std::cout << t.X << "," << t.Y << ";";
			if (t.X >= 0 && t.X < 500 && t.Y >= 0 && t.Y < 500) {
				cvMat.at<cv::Vec3b>(t.X, t.Y) = cv::Vec3b(255, 255, 255);
			}
		}
		cv::imshow("Circle", cvMat);
		return ans;
	}
	//绘制椭圆 a(x-cx)^2+b(y-cy)^2+c=0 (Brehensam 算法)
	static std::vector<std::vector<long long>> drawEllipse(long long a,long long b,long long c,long long centerX,long long centerY) {
		if ((a > 0 && b < 0) || (a < 0 && b>0) || a == 0 || b == 0 || (a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0)) {
			std::cout << "给定参数非法" << std::endl;
			return {};
		}
		//交换坐标轴
		long long doSwap = false;
		if (a > b) {
			doSwap = true;
			long long tmp = b;
			b = a;
			a = tmp;
		}
		//选择椭圆在右上1/4部分的上半部绘制
		std::vector<std::vector<long long>> ans;
		long long dx = 0, dy = round(sqrt(-c / b));
		long long delta = 2 * a + b * (dy * dy + (dy - 1) * (dy - 1)) + 2 * c;
		ans.push_back({ dx,dy });
		while (true) {
			if (delta < 0) { //如果delta<0,选择Hi(xi+1,yi)
				delta += 4 * a * dx + 6 * a;
				dx++;
			}
			else { //否则选择Li
				delta += 4 * a * dx + 6 * a - 4 * b * dy + 4 * b;
				dx++;
				dy--;
			}
			ans.push_back({ dx,dy });
			if (a * dx > b * dy) {
				break;
			}
		}
		//绘制右上1/4椭圆的下半部分
		dx = round(sqrt(-c / a));
		dy = 0;
		delta = a * (dx * dx + (dx - 1) * (dx - 1)) + 2 * b + 2 * c;
		ans.push_back({ dx,dy });
		while (true) {
			if (delta < 0) { //选择Hi(xi,yi+1)
				delta += 4 * b * dy + 6 * b;
				dy++;
			}
			else {
				delta += 4 * b * dy + 6 * b - 4 * a * dx + 4 * a;
				dy++;
				dx--;
			}
			ans.push_back({ dx,dy });
			if (a * dx <= b * dy) {
				break;
			}
		}
		//完成对称
		long long n = ans.size();
		for (long long i = 0; i < n; i++) {
			dx = ans[i][0];
			dy = ans[i][1];
			ans.push_back({ dx,-dy });
			ans.push_back({ -dx,-dy });
			ans.push_back({ -dx,dy });
		}
		//输出结果
		n = ans.size();
		cv::Mat cvMat = cv::Mat(500, 500, CV_8UC3);
		for (long long i = 0; i < n; i++) {
			if (!doSwap) {
				dx = ans[i][0] + centerX;
				dy = ans[i][1] + centerY;
			}
			else {
				dx = ans[i][1] + centerX;
				dy = ans[i][0] + centerY;
			}
			if (dx >= 0 && dx < 500 && dy >= 0 && dy < 500) {
				cvMat.at<cv::Vec3b>(dx, dy) = cv::Vec3b(255, 255, 255);
			}
			std::cout << dx << "," << dy << ";" ;
		}
		
		cv::imshow("Ellipse", cvMat);
		return ans;
	}
	//绘制椭圆 (x-x0)^2/a^2+(y-y0)^2/b^2=1
	static std::vector<std::vector<long long>> drawStdEllipse(long long a, long long b, long long centerX, long long centerY) {
		return drawEllipse(b * b, a * a, -b * b * a * a, centerX, centerY);
	}
	
};

int main() {
	std::string type;
	while (1) {
		std::cout << "请选择画圆(输入1),椭圆(输入2)或直线(输入3)" << std::endl;
		std::cin >> type;
		if (type == "1" || type == "2" || type == "3") {
			break;
		}
	}
	if (type == "1") {
		int cx, cy, r;
		while (1) {
			std::cout << "请输入圆心X坐标" << std::endl;
			std::cin >> cx;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入圆心Y坐标" << std::endl;
			std::cin >> cy;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入半径" << std::endl;
			std::cin >> r;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		Rasterizer::drawCircle(cx, cy, r);
	}
	if (type == "2") {
		int a, b, cx, cy;
		while (1) {
			std::cout << "请输入椭圆x^2/a^2+y^2/b^2=1中的a " << std::endl;
			std::cin >> a;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入椭圆x^2/a^2+y^2/b^2=1中的b " << std::endl;
			std::cin >> b;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入椭圆中心X坐标" << std::endl;
			std::cin >> cx;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入椭圆中心Y坐标" << std::endl;
			std::cin >> cy;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		Rasterizer::drawStdEllipse(a, b, cx, cy);
	}
	if (type == "3") {
		int cx, cy, r, dx, dy;
		while (1) {
			std::cout << "请输入起点X坐标" << std::endl;
			std::cin >> cx;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入起点Y坐标" << std::endl;
			std::cin >> cy;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入终点X坐标" << std::endl;
			std::cin >> dx;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入终点Y坐标" << std::endl;
			std::cin >> dy;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		while (1) {
			std::cout << "请输入宽度" << std::endl;
			std::cin >> r;
			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(0x7fffffff, '\n');
				std::cin.clear();
				continue;
			}
			break;
		}
		Rasterizer::drawLineW({ cx,cy }, { dx,dy }, r);
	}
	std::cout << std::endl;
	cv::waitKey(0);
	system("pause");
	return 0;
}
