#ifndef _VOLUME_MINKOWSKI_CONVEX_POLYHEDRON_UTILS_3_H_
#define _VOLUME_MINKOWSKI_CONVEX_POLYHEDRON_UTILS_3_H_

#include "AD.hpp"

// Convert a STL container containing 3D points to one single AD vector.
template < typename Vector,
           typename InputIterator
         >
Vector container_to_vector (InputIterator begin, InputIterator beyond) {
    int N = 0;
    for (InputIterator it = begin; it != beyond; ++it) {
        ++N;
    }

    Vector v(3 * N);
    int i = 0;
    for (InputIterator it = begin; it != beyond; ++it) {
        v(3 * i)     = AD(it->x(), 3 * N, 3 * i);
        v(3 * i + 1) = AD(it->y(), 3 * N, 3 * i + 1);
        v(3 * i + 2) = AD(it->z(), 3 * N, 3 * i + 2);
        ++i;
    }

    return v;
}

// Convert a vector to a container of 3D points.
template < typename Point,
           typename Vector,
           typename OutputIterator
         >
void vector_to_container (Vector const& v, OutputIterator out) {
    int N = v.rows() / 3;

    for (int i = 0; i < N; ++i) {
        *out++ = Point(v(3 * i), v(3 * i + 1), v(3 * i + 2));
    }
}

// Accumulate a value along the triangles composing the boundary
// of a polyhedron.
template < typename FT,
           typename Accum,
           typename Polyhedron
         >
FT accumulate_polyhedron_3 (Accum &acc, Polyhedron& P) {
    typedef typename Polyhedron::Halfedge_around_facet_circulator Hafc;
    typedef typename Polyhedron::Facet_iterator Facet_iterator;
    typedef typename Polyhedron::Point_3 Point_3;

    for (Facet_iterator fit = P.facets_begin();
         fit != P.facets_end();
         fit++) {
        Hafc h0 = fit->facet_begin(), hf = h0--, hs = hf;
        hs ++;

        while (1) {
            Point_3 a = h0->vertex()->point(),
                    b = hf->vertex()->point(),
                    c = hs->vertex()->point();

            acc(a, b, c);

            if (hs == h0)
                break;

            hs++; hf++;
        }
    }

    acc.end();

    return acc.value();
}

// Accumulator for computing the volume of a polyhedron.
template < typename FT,
           typename Point
         >
class VolumeAccumulator {
    public:
        VolumeAccumulator () : val(0) {}

        FT value () const {
            return val;
        }

        void operator() (Point a, Point b, Point c) {
            val += CGAL::cross_product(b - a, c - a) * (CGAL::ORIGIN - a);
        }

        void end () {
            val /= 6;
        }

    private:
        FT val;
};

// Compute the signed volume of a 3D polyhedron.
template < typename FT,
           typename Polyhedron
         >
FT signed_volume_polyhedron_3 (Polyhedron& P) {
    typedef typename Polyhedron::Point_3 Point_3;
    VolumeAccumulator<FT, Point_3> vacc;

    return accumulate_polyhedron_3<FT>(vacc, P);
}

// Compute the area of the boundary of a 3D polyhedron.
template < typename FT,
           typename Polyhedron
         >
FT area_boundary_polyhedron_3 (Polyhedron& P) {
    typedef typename Polyhedron::Halfedge_around_facet_circulator Hafc;
    typedef typename Polyhedron::Facet_iterator Facet_iterator;
    typedef typename Polyhedron::Point_3 Point_3;

    FT area = 0;

    for (Facet_iterator fit = P.facets_begin();
         fit != P.facets_end();
         fit++) {
        Hafc h0 = fit->facet_begin(), hf = h0--, hs = hf;
        hs ++;

        while (1) {
            Point_3 a = h0->vertex()->point(),
                    b = hf->vertex()->point(),
                    c = hs->vertex()->point();

            area += CGAL::sqrt(CGAL::cross_product(b - a, c - a).squared_length());
            // For AD
            /* area += sqrt(CGAL::cross_product(b - a, c - a).squared_length()); */

            if (hs == h0)
                break;

            hs++; hf++;
        }
    }

    return area / 2;
}

#endif
