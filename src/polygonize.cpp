#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <vector>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Traits = CGAL::Arr_segment_traits_2<K>;
using DCEL = CGAL::Arr_default_dcel<Traits>;
using Arr = CGAL::Arrangement_2<Traits, DCEL>;
using Segment = Arr::X_monotone_curve_2;
using Point = Arr::Point_2;

void adjacent_faces(const Arr::Face_const_handle &fit,
                    std::unordered_set<Arr::Face_const_handle> &res,
                    const Arr::Face_const_handle &outer_face) {
  assert(fit->has_outer_ccb());
  auto end = fit->outer_ccb();
  auto it = end;
  auto last_f = fit;
  do {
    auto adj_f = it->twin()->face();
    if (adj_f == outer_face) {
      continue;
    }
    assert(adj_f != fit);
    if (adj_f != last_f && res.count(adj_f) == 0) {

      res.insert(adj_f);
      adjacent_faces(adj_f, res, outer_face);
      last_f = adj_f;
    }
  } while (++it != end);
}

std::unordered_set<Arr::Face_const_handle> contained_faces(const Arr::Ccb_halfedge_const_circulator circ, const Arr::Face_const_handle &outer_face) {
  std::unordered_set<Arr::Face_const_handle> res;

  auto it = circ;
  auto last_f = circ->face();
  do {
    auto adj_f = it->twin()->face();
    if (adj_f == circ->face()) {
      continue;
    }
    if (adj_f != last_f && res.count(adj_f) == 0) {
      res.insert(adj_f);
      // run a depth first search to find all adjacent faces
      adjacent_faces(adj_f, res, outer_face);
    }
    last_f = adj_f;
  } while (++it != circ);

  res.erase(circ->face());

  return res;
}

std::size_t number_of_holes(const Arr::Face_const_handle &f_it) {
  std::size_t count = 0;
  for (auto it = f_it->holes_begin(), end = f_it->holes_end(); it != end; ++it) {
    ++count;
  }

  for (auto it = f_it->isolated_vertices_begin(), end = f_it->isolated_vertices_end(); it!= end; ++it) {
    ++count;
  }
  return count;
}

void output_poly(const Arr::Ccb_halfedge_const_circulator circ) {
  auto it = circ;
  auto p = it->source()->point();
  std::cout << p.x() << " " << p.y();
  do {
    auto p = it->target()->point();
    std::cout << " " << p.x() << " " << p.y();
  } while (++it != circ);
}

void output_vertex(const Arr::Vertex &v) {
  auto p = v.point();
  std::cout << p.x() << " " << p.y();
}

void output_subfaces(std::unordered_set<Arr::Face_const_handle> &subfaces,
                     std::unordered_map<Arr::Face_const_handle, std::size_t> &face_ids) {
  if (subfaces.size() == 0) {
    return;
  }

  auto it = subfaces.begin();
  auto end = subfaces.end();
  std::cout << face_ids.at(*it);
  for (++it; it != end; ++it) {
    std::cout << " " << face_ids.at(*it);
  }
}

void output_new(Arr &arr) {
  assert(arr.number_of_unbounded_faces() == 1);
  std::unordered_map<Arr::Face_const_handle, std::size_t> face_ids;
  for (auto it = arr.faces_begin(), end = arr.faces_end(); it != end; ++it) {
    if (it->is_unbounded()) {
      continue;
    }
    face_ids.emplace(std::make_pair(it, face_ids.size()+1));
  }

  std::cout.precision(10);

  std::queue<Arr::Face_const_handle> queue;
  queue.push(arr.unbounded_face());


  while (!queue.empty()) {
    auto f = queue.front();
    queue.pop();
    if (!f->is_unbounded()) {
      std::cout << face_ids.at(f) << " ";
      output_poly(f->outer_ccb());
      std::cout << std::endl;
    }
    auto hole_count = number_of_holes(f);
    std::cout << hole_count << std::endl;

    for (auto h_it = f->holes_begin(), h_end = f->holes_end(); h_it != h_end; ++h_it) {
      auto cf = contained_faces(*h_it, f);
      output_poly(*h_it);
      std::cout << std::endl;
      output_subfaces(cf, face_ids);
      std::cout << std::endl;
      for (auto f : cf) {
        queue.push(f);
      }
    }
    for (auto v_it = f->isolated_vertices_begin(), v_end = f->isolated_vertices_end(); v_it != v_end; ++v_it) {
      output_vertex(*v_it);
      std::cout << std::endl << std::endl;   // a vertex does not bound any face ...
    }
  }
}

int main() {
  std::vector<Segment> segs;

  double x1, y1, x2, y2;
  Arr arr;
  while (std::cin >> x1 >> y1 >> x2 >> y2) {
    Point p (x1, y1);
    Point q (x2, y2);
    if (p != q) {
      segs.emplace_back(p, q);
    } else {
      CGAL::insert_point(arr, p);
    }
  }

  CGAL::insert(arr, segs.begin(), segs.end());

 output_new(arr);
 return 0;
}
