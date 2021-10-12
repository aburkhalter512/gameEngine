use crate::engine::math::polygon::Polygon;
use crate::engine::math::vec::{Linearity, Vec2};

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
struct Segment<T>(pub Vec2<T>, pub Vec2<T>);

impl Segment<f64> {
    pub fn flip(&self) -> Self {
        Self(self.1, self.0)
    }

    pub fn intersects(&self, other: &Self) -> bool {
        let linearity = (
            Linearity::linearity(self.0, self.1, other.0),
            Linearity::linearity(self.0, self.1, other.1),
            Linearity::linearity(other.0, other.1, self.0),
            Linearity::linearity(other.0, other.1, self.1),
        );

        // ensure that each segment only has one point of the other segment to the left of it
        let segments_cross = (linearity.0 == Linearity::Clockwise)
            ^ (linearity.1 == Linearity::Clockwise)
            && (linearity.2 == Linearity::Clockwise) ^ (linearity.3 == Linearity::Clockwise);

        (linearity.0 == Linearity::Collinear && self.between(other.0))
            || (linearity.1 == Linearity::Collinear && self.between(other.1))
            || (linearity.2 == Linearity::Collinear && other.between(self.0))
            || (linearity.3 == Linearity::Collinear && other.between(self.1))
            || segments_cross
    }

    // precondition: self and other are collinear
    fn between(&self, other: Vec2<f64>) -> bool {
        let between_x = (self.0 .0 <= other.0 && other.0 <= self.1 .0)
            || (self.1 .0 <= other.0 && other.0 <= self.0 .0);
        let between_y = (self.0 .1 <= other.1 && other.1 <= self.1 .1)
            || (self.1 .1 <= other.1 && other.1 <= self.0 .1);

        between_x && between_y
    }
}

pub(crate) struct ReferenceVertex {
    pub prev: usize,
    pub next: usize,
    pub is_ear: bool,
}

pub struct ReferenceVertices<'a> {
    polygon: &'a Polygon,
    ear_vertices: Vec<ReferenceVertex>,
}

impl<'a> ReferenceVertices<'a> {
    pub fn new(polygon: &'a Polygon) {
        let vertices = polygon.vertices();
        let mut ear_vertices = Vec::new();
        ear_vertices.reserve(vertices.len());

        let last_vertex_index = vertices.len() - 1;

        // Wrap the first ear around the end of the vec
        ear_vertices.push(ReferenceVertex {
            prev: last_vertex_index,
            next: 1,
            is_ear: false,
        });

        for i in 1..last_vertex_index {
            ear_vertices.push(ReferenceVertex {
                prev: i - 1,
                next: i + 1,
                is_ear: false,
            });
        }

        // Wrap the last ear around the front of the vec
        ear_vertices.push(ReferenceVertex {
            prev: last_vertex_index - 1,
            next: 0,
            is_ear: false,
        })
    }

    pub(crate) fn is_diagonal(
        vertices: &Vec<Vec2<f64>>,
        ear_vertices: &Vec<ReferenceVertex>,
        base_index: usize,
        diagonal_index: usize,
    ) -> bool {
        let base_vertex = vertices[base_index];
        let diagonal_vertex = vertices[diagonal_index];
        let test_segment = Segment(base_vertex, diagonal_vertex);

        // Verify the suspect diagonal does not intersect with any of the polygon's own segments
        let mut first_vertex = ear_vertices[base_index].next;
        while ear_vertices[first_vertex].next != base_index {
            let next_vertex = ear_vertices[first_vertex].next;

            // Segments connected to other segments can never intersect (at least on the strict definition of intersecting)
            if first_vertex != diagonal_index && next_vertex != diagonal_index {
                let diagonal_segment = Segment(vertices[first_vertex], vertices[next_vertex]);

                // If any one polygon segment and the diagonal intersect, then this diagonal is not valid
                if test_segment.intersects(&diagonal_segment) {
                    return false;
                }
            }

            first_vertex = ear_vertices[first_vertex].next;
        }

        let prev_base_vertex = vertices[ear_vertices[base_index].prev];
        let next_base_vertex = vertices[ear_vertices[base_index].next];

        // Make sure the diagonal isn't external
        if Linearity::linearity(prev_base_vertex, base_vertex, next_base_vertex)
            == Linearity::CounterClockwise
        {
            // if the vertex is convex

            // Verify the diagonal is located inside the INTERNAL convex vertex
            // the idea here is to make sure the order of points appears in a counter clockwise fashion.
            Linearity::linearity(prev_base_vertex, base_vertex, diagonal_vertex)
                == Linearity::CounterClockwise
                && Linearity::linearity(base_vertex, next_base_vertex, diagonal_vertex)
                    == Linearity::CounterClockwise
        } else {
            // if the vertex is reflex

            // Verify the diagonal is NOT located inside the EXTERNAL convex vertex
            // the idea here is to make sure the order of points do not appear in a counter clockwise fashion.
            !(Linearity::linearity(base_vertex, diagonal_vertex, next_base_vertex)
                == Linearity::CounterClockwise
                && Linearity::linearity(next_base_vertex, base_vertex, prev_base_vertex)
                    == Linearity::CounterClockwise)
        }
    }
}

#[cfg(test)]
mod test {
    use crate::engine::math::{
        reference_vertices::{ReferenceVertex, ReferenceVertices},
        vec::Vec2,
    };

    use super::Segment;

    #[test]
    fn segment_intersects() {
        let segment1 = Segment(Vec2(0.25, 0.75), Vec2(0.75, 0.25));
        let segment2 = Segment(Vec2(0.25, 0.25), Vec2(0.75, 0.75));

        assert!(segment1.intersects(&segment2));
        assert!(segment2.intersects(&segment1));
        assert!(segment1.flip().intersects(&segment2));
        assert!(segment2.flip().intersects(&segment1));
    }

    #[test]
    fn segment_intersects_collinear_endpoint() {
        let segment1 = Segment(Vec2(0.25, 0.75), Vec2(0.75, 0.25));
        let segment2 = Segment(Vec2(0.5, 0.5), Vec2(0.75, 0.75));

        assert!(segment1.intersects(&segment2));
        assert!(segment2.intersects(&segment1));
        assert!(segment1.flip().intersects(&segment2));
        assert!(segment2.flip().intersects(&segment1));
    }

    #[test]
    fn segment_intersects_same_endpoint() {
        let segment1 = Segment(Vec2(0.25, 0.75), Vec2(0.75, 0.25));
        let segment2 = Segment(Vec2(0.25, 0.75), Vec2(0.75, 0.75));

        assert!(segment1.intersects(&segment2));
        assert!(segment2.intersects(&segment1));
        assert!(segment1.flip().intersects(&segment2));
        assert!(segment2.flip().intersects(&segment1));
    }

    #[test]
    fn segment_projection_does_not_intersect() {
        let segment1 = Segment(Vec2(0.25, 0.75), Vec2(0.75, 0.25));
        let segment2 = Segment(Vec2(-0.25, 0.75), Vec2(0.25, 1.25));

        assert!(!segment1.intersects(&segment2));
        assert!(!segment2.intersects(&segment1));
        assert!(!segment1.flip().intersects(&segment2));
        assert!(!segment2.flip().intersects(&segment1));
    }

    #[test]
    fn reference_vertices_is_diagonal_all_convex() {
        let vertices = vec![
            Vec2(0.0, 0.0),
            Vec2(1.0, 0.0),
            Vec2(1.0, 1.0),
            Vec2(0.0, 1.0),
        ];

        let ear_vertices = vec![
            ReferenceVertex {
                prev: 3,
                next: 1,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 0,
                next: 2,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 1,
                next: 3,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 2,
                next: 0,
                is_ear: true,
            },
        ];

        let expected_results = vec![(0, 2, true), (1, 3, true)];

        for (base, next, expect) in expected_results {
            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, base, next),
                expect
            );

            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, next, base),
                expect
            );
        }
    }

    #[test]
    fn reference_vertices_is_diagonal_single_reflex() {
        let vertices = vec![
            Vec2(0.0, 0.0),
            Vec2(1.0, 1.0),
            Vec2(2.0, 0.0),
            Vec2(1.0, 2.0),
        ];

        let ear_vertices = vec![
            ReferenceVertex {
                prev: 3,
                next: 1,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 0,
                next: 2,
                is_ear: false,
            },
            ReferenceVertex {
                prev: 1,
                next: 3,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 2,
                next: 0,
                is_ear: true,
            },
        ];

        let expected_results = vec![(0, 2, false), (1, 3, true)];

        for (base, next, expect) in expected_results {
            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, base, next),
                expect
            );

            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, next, base),
                expect
            );
        }
    }

    #[test]
    fn reference_vertices_is_diagonal_internal_intersection() {
        let vertices = vec![
            Vec2(0.0, 0.0),
            Vec2(1.0, 1.0),
            Vec2(2.0, 0.0),
            Vec2(2.0, 1.0),
            Vec2(1.0, 2.0),
        ];

        let ear_vertices = vec![
            ReferenceVertex {
                prev: 4,
                next: 1,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 0,
                next: 2,
                is_ear: false,
            },
            ReferenceVertex {
                prev: 1,
                next: 3,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 2,
                next: 4,
                is_ear: true,
            },
            ReferenceVertex {
                prev: 3,
                next: 0,
                is_ear: true,
            },
        ];

        let expected_results = vec![
            (0, 2, false),
            (0, 3, false),
            (1, 3, true),
            (1, 4, true),
            (2, 4, true),
        ];

        for (base, next, expect) in expected_results {
            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, base, next),
                expect
            );

            assert_eq!(
                ReferenceVertices::is_diagonal(&vertices, &ear_vertices, next, base),
                expect
            );
        }
    }
}
