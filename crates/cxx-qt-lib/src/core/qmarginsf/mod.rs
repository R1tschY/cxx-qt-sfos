#[cfg(qt_coord_type = "float")]
mod qmarginsf32;
#[cfg(not(qt_coord_type = "float"))]
mod qmarginsf64;

#[cfg(qt_coord_type = "float")]
pub use qmarginsf32::*;
#[cfg(not(qt_coord_type = "float"))]
pub use qmarginsf64::*;
