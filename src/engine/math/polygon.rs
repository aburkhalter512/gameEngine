use crate::engine::math::vec::Vec2;

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    InsufficientVertices,
}

#[derive(Debug)]
pub struct Polygon(Vec<Vec2<f64>>);

impl Polygon {
    pub fn new(vertices: Vec<Vec2<f64>>) -> Result<Self, Error> {
        if vertices.len() < 3 {
            Err(Error::InsufficientVertices)
        } else {
            Ok(Self(vertices))
        }
    }

    pub fn translate(mut self, translation: Vec2<f64>) -> Self {
        for vertex in &mut self.0 {
            *vertex = *vertex + translation;
        }

        self
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn polygon_create() {
        Polygon::new(vec![Vec2(0.0, 0.0), Vec2(0.0, 0.0), Vec2(0.0, 0.0)]).unwrap();
    }

    #[test]
    fn polygon_create_fails_insufficient_vertices() {
        let err = Polygon::new(vec![Vec2(0.0, 0.0), Vec2(0.0, 0.0)]).unwrap_err();
        assert_eq!(err, Error::InsufficientVertices);
    }
}
