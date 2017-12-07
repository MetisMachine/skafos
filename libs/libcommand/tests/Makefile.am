AUTOMAKE_OPTIONS = subdir-objects

TEST_PROGS = \
	descriptive/holds_data.test \
	callable/invokes_provided_function.test \
	callable/invokes_void_function.test \
	callable/invokes_class_method.test \
	parameter/is_descriptive.test \
	parameter/should_be_non_required.test \
	argument/handles_string_value.test \
	argument/handles_int_value.test \
	argument/handles_negative_int_value.test \
	argument/handles_float_value.test \
	argument/handles_negative_float_value.test \
	argument/handles_boolean_value.test \
	option/handles_string_value.test \
	option/handles_int_value.test \
	option/handles_negative_int_value.test \
	option/handles_float_value.test \
	option/handles_negative_float_value.test \
	option/handles_boolean_value.test \
	option/handles_void_value.test \
	option/should_match_exact_name.test \
	option/should_throw_exception_on_missing_value.test \
	required/should_be_required.test \
	multivalue/should_extract_arguments_by_separator.test \
	multivalue/should_extract_options_by_separator.test

TEST_SCRPTS = \
	command/understand_void_option.test \
	command/understand_bool_option.test \
	command/understand_int_option.test \
	command/understand_unsigned_int_option.test

TESTS = \
	$(TEST_PROGS) \
	$(TEST_SCRPTS)

EXTRA_DIST = \
	$(TEST_SCRPTS)

noinst_PROGRAMS = \
	$(TEST_PROGS) \
	command/option_test_command

AM_CXXFLAGS = -I$(top_srcdir)/include -std=c++11

check-% :	%.test all
	@srcdir=$(srcdir); export srcdir;

cov-reset:
	rm -fr coverage
	find . -name "*.gcda" -exec rm {} \;
	find . -name "*.gcno" -exec rm {} \;
	lcov --directory . --zerocounters

cov-report:
	mkdir -p coverage
	lcov --compat-libtool --directory . --capture --output-file coverage/app.info
	genhtml -o coverage/ coverage/app.info

cov:
	make cov-report

clean-local:
	make cov-reset

descriptive_holds_data_test_SOURCES  = descriptive/holds_data.cpp

callable_invokes_provided_function_test_SOURCES  = callable/invokes_provided_function.cpp
callable_invokes_void_function_test_SOURCES  = callable/invokes_void_function.cpp
callable_invokes_class_method_test_SOURCES  = callable/invokes_class_method.cpp

parameter_is_descriptive_test_SOURCES  = parameter/is_descriptive.cpp
parameter_should_be_non_required_test_SOURCES = parameter/should_be_non_required.cpp

argument_handles_string_value_test_SOURCES  = argument/handles_string_value.cpp
argument_handles_int_value_test_SOURCES  = argument/handles_int_value.cpp
argument_handles_negative_int_value_test_SOURCES  = argument/handles_negative_int_value.cpp
argument_handles_float_value_test_SOURCES  = argument/handles_float_value.cpp
argument_handles_negative_float_value_test_SOURCES  = argument/handles_negative_float_value.cpp
argument_handles_boolean_value_test_SOURCES  = argument/handles_boolean_value.cpp

option_handles_string_value_test_SOURCES  = option/handles_string_value.cpp
option_handles_int_value_test_SOURCES = option/handles_int_value.cpp
option_handles_negative_int_value_test_SOURCES = option/handles_negative_int_value.cpp
option_handles_float_value_test_SOURCES  = option/handles_float_value.cpp
option_handles_negative_float_value_test_SOURCES  = option/handles_negative_float_value.cpp
option_handles_boolean_value_test_SOURCES  = option/handles_boolean_value.cpp
option_handles_void_value_test_SOURCES  = option/handles_void_value.cpp
option_should_match_exact_name_test_SOURCES  = option/should_match_exact_name.cpp
option_should_throw_exception_on_missing_value_test_SOURCES = option/should_throw_exception_on_missing_value.cpp

required_should_be_required_test_SOURCES = required/should_be_required.cpp

multivalue_should_extract_arguments_by_separator_test_SOURCES = multiValue/should_extract_arguments_by_separator.cpp
multivalue_should_extract_options_by_separator_test_SOURCES = multiValue/should_extract_options_by_separator.cpp

command_option_test_command_SOURCES = command/src/option_test_command.cpp
