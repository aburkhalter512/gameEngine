use std::ops::Add;

#[derive(Debug, Clone, Copy)]
pub struct Vec2<T>(pub T, pub T);

impl<T: Add<Output = T> + Copy> Add for Vec2<T> {
    type Output = Vec2<T>;

    fn add(self, rhs: Vec2<T>) -> Vec2<T> {
        Vec2(self.0 + rhs.0, self.1 + rhs.1)
    }
}

