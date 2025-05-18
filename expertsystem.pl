% Entry point
start :-
    write('Car Troubleshooting Expert System'), nl,
    write('Answer the following questions with yes or no.'), nl,
    diagnose_best(Problem),
    write('Based on the symptoms, the problem might be: '), write(Problem), nl,
    undo.

% Problem definitions with their symptoms (more shared symptoms added)
problem(dead_battery, [
    car_wont_start,
    dim_lights,
    clicking_sound_when_turning_key,
    battery_voltage_low,
    corrosion_on_battery_terminals,
    battery_warning_light_on
]).

problem(alternator_failure, [
    battery_dies_after_driving,
    battery_warning_light_on,
    electrical_components_malfunction,
    dim_lights,
    poor_fuel_efficiency
]).

problem(flat_tire, [
    car_leans_on_one_side,
    thumping_noise_while_driving,
    tire_pressure_low_on_one_side
]).

problem(broken_starter, [
    car_wont_start,
    no_sound_when_turning_key,
    lights_work,
    dim_lights,
    clicking_sound_when_turning_key
]).

problem(out_of_fuel, [
    engine_cranks_but_doesnt_start,
    fuel_gauge_empty,
    no_fuel_smell,
    car_wont_start
]).

problem(spark_plug_issue, [
    engine_hard_to_start,
    poor_fuel_efficiency,
    engine_misfires,
    engine_cranks_but_doesnt_start,
    engine_stalls_when_hot
]).

problem(overheated_engine, [
    engine_temperature_high,
    coolant_leak,
    steam_from_hood,
    engine_stalls_when_hot
]).

problem(broken_suspension, [
    car_leans_on_one_side,
    thumping_noise_while_driving,
    uneven_tire_wear
]).

problem(cooling_system_failure, [
    engine_temperature_high,
    coolant_leak,
    radiator_fan_not_working
]).

% Diagnose by selecting problem with most matching symptoms
diagnose_best(BestProblem) :-
    findall(
        Score-Problem,
        (
            problem(Problem, Symptoms),
            count_matched_symptoms(Symptoms, Score)
        ),
        Scores
    ),
    sort(Scores, SortedScores),      % sort ascending
    reverse(SortedScores, [BestScore-BestProblem|_]),
    BestScore > 0, !.                % must have at least one symptom matched

diagnose_best(unknown).

% Count how many symptoms match user confirmed symptoms
count_matched_symptoms(Symptoms, Count) :-
    include(symptom, Symptoms, Matched),
    length(Matched, Count).

% Check symptom: known or ask user
symptom(Symptom) :- known(Symptom, true), !.
symptom(Symptom) :- known(Symptom, false), !, fail.
symptom(Symptom) :- ask(Symptom).

% Ask user-friendly questions
ask(Symptom) :-
    symptom_question(Symptom, Question),
    format('~w (yes/no): ', [Question]),
    read(Response),
    nl,
    ( (Response == yes ; Response == y)
      -> asserta(known(Symptom, true))
      ;  asserta(known(Symptom, false)), fail).

% User-friendly questions for symptoms
symptom_question(car_wont_start, 'Does the car fail to start at all?').
symptom_question(dim_lights, 'Are the lights dim when you try to start the car?').
symptom_question(clicking_sound_when_turning_key, 'Do you hear a clicking sound when turning the key?').
symptom_question(battery_voltage_low, 'Is the battery voltage below 12 volts?').
symptom_question(corrosion_on_battery_terminals, 'Is there visible corrosion on the battery terminals?').
symptom_question(battery_warning_light_on, 'Is the battery warning light on the dashboard lit?').
symptom_question(battery_dies_after_driving, 'Does the battery die after driving for some time?').
symptom_question(electrical_components_malfunction, 'Are electrical components like radio, lights, or power windows malfunctioning?').
symptom_question(car_leans_on_one_side, 'Does the car lean to one side?').
symptom_question(thumping_noise_while_driving, 'Do you hear a thumping noise while driving?').
symptom_question(tire_pressure_low_on_one_side, 'Is the tire pressure low on one side?').
symptom_question(engine_cranks_but_doesnt_start, 'Does the engine crank but fail to start?').
symptom_question(fuel_gauge_empty, 'Is the fuel gauge showing empty?').
symptom_question(no_fuel_smell, 'Do you notice a smell of fuel near the car?').
symptom_question(no_sound_when_turning_key, 'Is there no sound at all when turning the key?').
symptom_question(lights_work, 'Do the lights come on when you turn the key?').
symptom_question(engine_hard_to_start, 'Is the engine hard to start?').
symptom_question(poor_fuel_efficiency, 'Has the fuel efficiency dropped recently?').
symptom_question(engine_misfires, 'Does the engine misfire or run roughly?').
symptom_question(engine_temperature_high, 'Is the engine temperature gauge higher than normal?').
symptom_question(coolant_leak, 'Is there a coolant leak visible under the car?').
symptom_question(steam_from_hood, 'Do you see steam coming from under the hood?').
symptom_question(engine_stalls_when_hot, 'Does the engine stall when it gets hot?').
symptom_question(broken_suspension, 'Does the car have issues with suspension like bumps or noises?').
symptom_question(uneven_tire_wear, 'Are the tires worn unevenly?').
symptom_question(radiator_fan_not_working, 'Is the radiator fan not working?').

% Clear known facts after diagnosis
undo :- retract(known(_, _)), fail.
undo.

% Declare known/2 as dynamic
:- dynamic known/2.
