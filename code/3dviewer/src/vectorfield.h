#ifndef _VECTORFIELD_H_
#define _VECTORFIELD_H_

#include <map>

template <typename T>
static T clamp (T const& val, T const& m, T const& M) {
    if (val <= m) {
        return m;
    } else if (val >= M) {
        return M;
    } else {
        return val;
    }
}

static void ramp (double val,
           double &r,
           double &g,
           double &b) {
    val = clamp(val, 0.0, 1.0);

    b = 0;

    if (val <= 0.5) {
        g = 1.0;
        r = 2 * val;
    } else {
        r = 1.0;
        g = 1.0 - 2 * (val - 0.5);
    }
}

template <typename Kernel>
class CVector_field {
    public:
        typedef typename Kernel::Point_3 Point;
        typedef typename Kernel::Vector_3 Vector;

    public:
        // LIFE CYCLE ============================================
        CVector_field () {
        }

        template <typename PointIterator, typename VectorIterator>
        CVector_field (PointIterator pbegin,
                       PointIterator pbeyond,
                       VectorIterator vbegin,
                       VectorIterator vbeyond) {
            addPoints(pbegin, pbeyond, vbegin, vbeyond);
        }

        // SIZE ==================================================

        size_t size () const {
            return vector_field.size();
        }

        // ADD / REMOVE VECTORS ==================================
        void addVector (Point p, Vector v) {
            vector_field[p] = v;
        }

        template <typename PointIterator, typename VectorIterator>
        void addVectors (PointIterator pbegin,
                         PointIterator pbeyond,
                         VectorIterator vbegin,
                         VectorIterator vbeyond) {
            PointIterator pit = pbegin;
            VectorIterator vit = vbegin;

            while (pit != pbeyond && vit != vbeyond) {
                vector_field[*pit] = *vit;

                ++pit;
                ++vit;
            }
        }

        void clear () {
            vector_field.clear();
        }

        // ACCESS VECTORS ==================================
        Vector& operator[] (Point const& p) {
            return vector_field[p];
        }

        // RENDERING =============================================
        // draw vecor field
        void gl_draw_field (const float line_width,
                            const unsigned char r,
                            const unsigned char g,
                            const unsigned char b) {
            ::glBegin(GL_LINES);

            ::glLineWidth(line_width);
            /* ::glColor3ub(r, g, b); */

            double max_value = 0;
            // Find the maximum
            for (typename std::map<Point, Vector>::iterator mit = vector_field.begin();
                 mit != vector_field.end();
                 ++mit) {
                if (mit->second.squared_length() > max_value * max_value) {
                    max_value = CGAL::sqrt(mit->second.squared_length());
                }
            }

            for (typename std::map<Point, Vector>::iterator mit = vector_field.begin();
                 mit != vector_field.end();
                 ++mit) {
                Point p = mit->first;
                Vector v = mit->second;
                /* v = v / CGAL::sqrt(v.squared_length()); */
                Point q = p + v;
                /* Point q = p + v; */

                double rr, gg, bb;
                ramp(CGAL::sqrt(mit->second.squared_length()) / max_value, rr, gg, bb);
                ::glColor3f(rr, gg, bb);
                ::glVertex3d(p[0], p[1], p[2]);
                ::glVertex3d(q[0], q[1], q[2]);
            }

            ::glEnd();
        }

    private:
        std::map<Point, Vector> vector_field;
};

#endif

