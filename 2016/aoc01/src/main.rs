use std::collections::HashMap;
use std::{fs, io, process};
use std::io::BufRead;

enum Direction {
    Right,
    Left
}

struct Instruction {
    direction: Direction,
    steps: i32,
}

fn read_instructions(filename: &str) -> Vec<Instruction> {
    let mut vec: Vec<Instruction> = Vec::new();

    let file = fs::File::open(filename).expect("File not found");

    let lines = io::BufReader::new(file).lines();
    for line_res in lines {
        if let Ok(line) = line_res {
            let instructions = line.split(", ");
            for instruction in instructions {
                let direction = match instruction.chars().nth(0) {
                    Some('L') => Direction::Left,
                    Some('R') => Direction::Right,
                    _ => process::exit(1), // not elegant
                };
                vec.push(Instruction{direction: direction, steps: instruction.to_string()[1..].parse::<i32>().unwrap()});
            }
        }
    }
    return vec;
}

fn part1(instructions: &Vec<Instruction>) -> i32 {
    let mut position = (0,0);
    let mut direction = (1,0);
    for instruction in instructions {
        direction = match instruction.direction {
            Direction::Right => (-direction.1, direction.0),
            Direction::Left => (direction.1, -direction.0),
        };
        position = (position.0 + instruction.steps * direction.0, position.1 + instruction.steps * direction.1);
    }

    return position.0.abs() + position.1.abs();
}

fn part2(instructions: &Vec<Instruction>) -> i32 {
    let mut position = (0,0);
    let mut direction = (1,0);
    let mut map: HashMap<(i32, i32), i32> = HashMap::new();
    map.insert((0,0), 1);
    for instruction in instructions {
        direction = match instruction.direction {
            Direction::Right => (-direction.1, direction.0),
            Direction::Left => (direction.1, -direction.0),
        };
        for _ in 0..instruction.steps {
            position = (position.0 + direction.0, position.1 + direction.1);
            if map.contains_key(&position) {
                return position.0.abs() + position.1.abs();
            } else {
                map.insert(position, 1);
            }
        }
    }

    return -1;
}

fn main() {
    let instructions = read_instructions("input");
    println!("The Easter Bunny HQ is {} blocks away", part1(&instructions));
    println!("The Easter Bunny HQ is actually {} blocks away", part2(&instructions));
}
