using DelaunatorSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2D_Contour_Plotter
{
    public class DelaunayInterpolator
    {
        private Delaunator delaunator;
        private List<double> xCoords;
        private List<double> yCoords;
        private List<double> zValues;

        public DelaunayInterpolator(List<double> x, List<double> y, List<double> z)
        {
            xCoords = x;
            yCoords = y;
            zValues = z;

            // var points = x.Zip(y, (xi, yi) => new DelaunatorSharp.Point((float)xi, (float)yi)).ToArray();

            IPoint[] points = x.Zip(y, (xi, yi) => new Point(xi, yi)).Cast<IPoint>().ToArray();

            delaunator = new Delaunator(points);
        }

        public double? Interpolate(double xi, double yi)
        {
            var triangles = delaunator.Triangles;
            var coords = delaunator.Points;

            for (int i = 0; i < triangles.Length; i += 3)
            {
                int t0 = triangles[i];
                int t1 = triangles[i + 1];
                int t2 = triangles[i + 2];

                var p0 = coords[t0];
                var p1 = coords[t1];
                var p2 = coords[t2];

                if (IsPointInTriangle(xi, yi, p0, p1, p2))
                {
                    double z0 = zValues[t0];
                    double z1 = zValues[t1];
                    double z2 = zValues[t2];

                    return BarycentricInterpolation(xi, yi, p0, p1, p2, z0, z1, z2);
                }
            }

            // Point is outside the convex hull
            return null;
        }

        private bool IsPointInTriangle(double px, double py, IPoint v0, IPoint v1, IPoint v2)
        {
            double dX = px - v2.X;
            double dY = py - v2.Y;
            double dX21 = v2.X - v1.X;
            double dY12 = v1.Y - v2.Y;
            double D = dY12 * (v0.X - v2.X) + dX21 * (v0.Y - v2.Y);
            double s = dY12 * dX + dX21 * dY;
            double t = (v2.Y - v0.Y) * dX + (v0.X - v2.X) * dY;

            if (D < 0)
                return s <= 0 && t <= 0 && s + t >= D;
            return s >= 0 && t >= 0 && s + t <= D;
        }

        private double BarycentricInterpolation(double px, double py, IPoint v0, IPoint v1, IPoint v2,
            double z0, double z1, double z2)
        {
            double det = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
            double a = ((v1.Y - v2.Y) * (px - v2.X) + (v2.X - v1.X) * (py - v2.Y)) / det;
            double b = ((v2.Y - v0.Y) * (px - v2.X) + (v0.X - v2.X) * (py - v2.Y)) / det;
            double c = 1.0 - a - b;

            return a * z0 + b * z1 + c * z2;
        }


    }

}
