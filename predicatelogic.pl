% Facts
course(math101).
course(cs101).
course(cs201).
course(cs301).
course(cs401).

% Course capacity (max students)
capacity(math101, 30).
capacity(cs101, 25).
capacity(cs201, 20).
capacity(cs301, 15).
capacity(cs401, 10).

% Current enrollments count
enrolled_count(math101, 28).
enrolled_count(cs101, 20).
enrolled_count(cs201, 20).
enrolled_count(cs301, 14).
enrolled_count(cs401, 9).

% Prerequisites (direct)
prerequisite(cs201, math101).
prerequisite(cs201, cs101).
prerequisite(cs301, cs201).
prerequisite(cs401, cs301).

% Co-requisites (must enroll together)
corequisite(cs301, cs201).

% Students info
has_completed(john, math101).
has_completed(john, cs101).
has_completed(john, cs201).

has_completed(michael, cs101).

student_year(john, 3).
student_year(michael, 2).

% GPA info (on scale 0-4)
student_gpa(john, 3.5).
student_gpa(michael, 2.8).

% Predicate to check all prerequisites recursively
all_prerequisites(Course, AllPrereqs) :-
    findall(P, prerequisite(Course, P), DirectPrereqs),
    findall(SubP, (member(C, DirectPrereqs), all_prerequisites(C, SubPList), member(SubP, SubPList)), SubPrereqs),
    append(DirectPrereqs, SubPrereqs, All),
    sort(All, AllPrereqs).

all_prerequisites(Course, []) :-
    \+ prerequisite(Course, _).

% Check if student completed all prerequisites (recursive)
has_completed_all_prerequisites(Student, Course) :-
    all_prerequisites(Course, Prereqs),
    forall(member(P, Prereqs), has_completed(Student, P)).

% Check if student meets year and GPA requirement (example)
meets_year_and_gpa(Student, Course) :-
    student_year(Student, Year),
    student_gpa(Student, GPA),
    % Example rule: cs401 requires year >= 4 and GPA >= 3.0
    (Course = cs401 -> Year >= 4, GPA >= 3.0 ; true).

% Check if course is not full
course_not_full(Course) :-
    capacity(Course, Cap),
    enrolled_count(Course, Count),
    Count < Cap.

% Check co-requisites enrolled or being enrolled simultaneously
check_corequisites(Student, Course) :-
    \+ corequisite(Course, _);
    (
        corequisite(Course, CoReq),
        (has_completed(Student, CoReq); can_enroll(Student, CoReq))
    ).

% Enhanced can_enroll predicate
can_enroll(Student, Course) :-
    course(Course),
    \+ has_completed(Student, Course),
    has_completed_all_prerequisites(Student, Course),
    meets_year_and_gpa(Student, Course),
    course_not_full(Course),
    check_corequisites(Student, Course).

% Suggest enrollments
suggest_enrollment(Student) :-
    can_enroll(Student, Course),
    format('~w can enroll in ~w~n', [Student, Course]),
    fail.
suggest_enrollment(_).

% User interaction
start :-
    write('Enter student name: '),
    read(Student),
    suggest_enrollment(Student).
