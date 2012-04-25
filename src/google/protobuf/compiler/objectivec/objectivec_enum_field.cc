// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.
// http://code.google.com/p/protobuf/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Author: cyrusn@google.com (Cyrus Najmabadi)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <map>
#include <string>

#include <google/protobuf/compiler/objectivec/objectivec_enum_field.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/objectivec/objectivec_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace objectivec {

namespace {
void SetEnumVariables(const FieldDescriptor* descriptor,
		map<string, string>* variables) {
	const EnumValueDescriptor* default_value;
	default_value = descriptor->default_value_enum();

	string type = ClassName(descriptor->enum_type());

	(*variables)["classname"] = ClassName(descriptor->containing_type());
	(*variables)["name"] = UnderscoresToCamelCase(descriptor);
	(*variables)["capitalized_name"] = UnderscoresToCapitalizedCamelCase(
			descriptor);
	(*variables)["number"] = SimpleItoa(descriptor->number());
	(*variables)["type"] = type;
	(*variables)["default"] = EnumValueName(default_value);
	(*variables)["boxed_value"] = "[NSNumber numberWithInt:_value]";
	(*variables)["unboxed_value"] = "[_value intValue]";
	(*variables)["tag"] = SimpleItoa(internal::WireFormat::MakeTag(descriptor));
	(*variables)["tag_size"] = SimpleItoa(
			internal::WireFormat::TagSize(descriptor->number(),
					descriptor->type()));
}
} // namespace

EnumFieldGenerator::EnumFieldGenerator(const FieldDescriptor* descriptor) :
		descriptor_(descriptor) {
	SetEnumVariables(descriptor, &variables_);
}

EnumFieldGenerator::~EnumFieldGenerator() {
}

void EnumFieldGenerator::GenerateHasFieldHeader(io::Printer* printer) const {
	printer->Print(variables_, "BOOL has$capitalized_name$_:1;\n");
}

void EnumFieldGenerator::GenerateFieldHeader(io::Printer* printer) const {
	printer->Print(variables_, "$type$ $name$;\n");
}

void EnumFieldGenerator::GenerateHasPropertyHeader(io::Printer* printer) const {
	printer->Print(variables_, "- (BOOL) has$capitalized_name$;\n");
}

void EnumFieldGenerator::GeneratePropertyHeader(io::Printer* printer) const {
	printer->Print(variables_,
			"@property (nonatomic, readwrite) $type$ $name$;\n");
}

void EnumFieldGenerator::GenerateExtensionSource(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateMembersHeader(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateMembersSource(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateSynthesizeSource(io::Printer* printer) const {
	printer->Print(variables_, "- (BOOL) has$capitalized_name$ {\n"
			"  return !!has$capitalized_name$_;\n"
			"}\n"
			"- (void) setHas$capitalized_name$:(BOOL) _value {\n"
			"  has$capitalized_name$_ = !!_value;\n"
			"}\n"
			"@synthesize $name$;\n");
}

void EnumFieldGenerator::GenerateDeallocSource(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateInitializationSource(
		io::Printer* printer) const {
	printer->Print(variables_, "self.$name$ = $default$;\n");
}

void EnumFieldGenerator::GenerateBuilderMembersHeader(
		io::Printer* printer) const {
	printer->Print(variables_, "- (BOOL) has$capitalized_name$;\n"
			"- ($type$) $name$;\n"
			"- ($classname$_Builder*) set$capitalized_name$:($type$) _value;\n"
			"- ($classname$_Builder*) clear$capitalized_name$;\n");
}

void EnumFieldGenerator::GenerateBuilderMembersSource(
		io::Printer* printer) const {
	printer->Print(variables_,
			"- (BOOL) has$capitalized_name$ {\n"
					"  return protobufBuilderResult.has$capitalized_name$;\n"
					"}\n"
					"- ($type$) $name$ {\n"
					"  return protobufBuilderResult.$name$;\n"
					"}\n"
					"- ($classname$_Builder*) set$capitalized_name$:($type$) _value {\n"
					"  if (_value != nil) protobufBuilderResult.has$capitalized_name$ = true;\n"
					"  else protobufBuilderResult.has$capitalized_name$ = false;\n"
					"  protobufBuilderResult.$name$ = _value;\n"
					"  return self;\n"
					"}\n"
					"- ($classname$_Builder*) clear$capitalized_name$ {\n"
					"  protobufBuilderResult.$name$ = $default$;\n"
					"  protobufBuilderResult.has$capitalized_name$ = NO;\n"
					"  return self;\n"
					"}\n");
}

void EnumFieldGenerator::GenerateMergingCodeHeader(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateMergingCodeSource(io::Printer* printer) const {
	printer->Print(variables_, "if (other.has$capitalized_name$) {\n"
			"  [self set$capitalized_name$:other.$name$];\n"
			"}\n");
}

void EnumFieldGenerator::GenerateBuildingCodeHeader(
		io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateBuildingCodeSource(
		io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateParsingCodeHeader(io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateParsingCodeSource(io::Printer* printer) const {
	printer->Print(variables_, "int32_t _value = [input readEnum];\n"
			"if ($type$IsValidValue(_value)) {\n"
			"  [self set$capitalized_name$:_value];\n"
			"} else {\n"
			"  [unknownFields mergeVarintField:$number$ value:_value];\n"
			"}\n");
}

void EnumFieldGenerator::GenerateSerializationCodeHeader(
		io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateSerializationCodeSource(
		io::Printer* printer) const {
	printer->Print(variables_, "if (self.has$capitalized_name$) {\n"
			"  [output writeEnum:$number$ value:self.$name$];\n"
			"}\n");
}

void EnumFieldGenerator::GenerateSerializedSizeCodeHeader(
		io::Printer* printer) const {
}

void EnumFieldGenerator::GenerateSerializedSizeCodeSource(
		io::Printer* printer) const {
	printer->Print(variables_, "if (self.has$capitalized_name$) {\n"
			"  size += computeEnumSize($number$, self.$name$);\n"
			"}\n");
}

string EnumFieldGenerator::GetBoxedType() const {
	return ClassName(descriptor_->enum_type());
}

RepeatedEnumFieldGenerator::RepeatedEnumFieldGenerator(
		const FieldDescriptor* descriptor) :
		descriptor_(descriptor) {
	SetEnumVariables(descriptor, &variables_);
}

RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {
}

void RepeatedEnumFieldGenerator::GenerateHasFieldHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateFieldHeader(
		io::Printer* printer) const {
	printer->Print(variables_, "NSMutableArray* $name$;\n");
	if (descriptor_->options().packed()) {
		printer->Print(variables_, "int32_t $name$MemoizedSerializedSize;\n");
	}
}

void RepeatedEnumFieldGenerator::GenerateHasPropertyHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GeneratePropertyHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateExtensionSource(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateSynthesizeSource(
		io::Printer* printer) const {
	printer->Print(variables_, "@synthesize $name$;\n");
}

void RepeatedEnumFieldGenerator::GenerateDeallocSource(
		io::Printer* printer) const {
	printer->Print(variables_, "self.$name$ = nil;\n");
}

void RepeatedEnumFieldGenerator::GenerateInitializationSource(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateMembersHeader(
		io::Printer* printer) const {
	printer->Print(variables_,
			"@property (nonatomic, retain) NSMutableArray* $name$;\n"
					"- ($type$) $name$AtIndex:(int32_t) index;\n");
}

void RepeatedEnumFieldGenerator::GenerateBuilderMembersHeader(
		io::Printer* printer) const {
	printer->Print(variables_,
			"- (NSArray*) $name$;\n"
					"- ($type$) $name$AtIndex:(int32_t) index;\n"
					"- ($classname$_Builder*) replace$capitalized_name$AtIndex:(int32_t) index with:($type$) _value;\n"
					"- ($classname$_Builder*) add$capitalized_name$:($type$) _value;\n"
					"- ($classname$_Builder*) addAll$capitalized_name$:(NSArray*) values;\n"
					"- ($classname$_Builder*) clear$capitalized_name$List;\n");
}

void RepeatedEnumFieldGenerator::GenerateMergingCodeHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateBuildingCodeHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateParsingCodeHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateSerializationCodeHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateSerializedSizeCodeHeader(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateMembersSource(
		io::Printer* printer) const {
	printer->Print(variables_, "- (NSMutableArray*) $name$ {\n"
			"  return $name$;\n"
			"}\n"
			"- (void) set$capitalized_name$:(NSArray*) _value {\n"
			"  if ($name$ == _value) return;\n\n"
			"  [$name$ autorelease];\n"
			"  if (_value != nil) {\n"
			"    $name$ = [[NSMutableArray arrayWithArray:_value] retain];\n\n"
			"    has$capitalized_name$ = true;\n"
			"  }\n"
			"  else {\n"
			"    $name = nil;\n"
			"    has$capitalized_name$ = false;\n"
			"  }\n"
			"}\n"
			"- ($type$) $name$AtIndex:(int32_t) index {\n"
			"  NSNumber* _value = [$name$ objectAtIndex:index];\n"
			"  return [_value intValue];\n"
			"}\n");
}

void RepeatedEnumFieldGenerator::GenerateBuilderMembersSource(
		io::Printer* printer) const {
	printer->Print(variables_,
			"- (NSArray*) $name$ {\n"
					"  return protobufBuilderResult.$name$;\n"
					"}\n"
					"- ($type$) $name$AtIndex:(int32_t) index {\n"
					"  return [protobufBuilderResult $name$AtIndex:index];\n"
					"}\n"
					"- ($classname$_Builder*) replace$capitalized_name$AtIndex:(int32_t) index with:($type$) _value {\n"
					"  [protobufBuilderResult.$name$ replaceObjectAtIndex:index withObject:[NSNumber numberWithInt:_value]];\n"
					"  return self;\n"
					"}\n"
					"- ($classname$_Builder*) add$capitalized_name$:($type$) _value {\n"
					"  if (protobufBuilderResult.$name$ == nil) {\n"
					"    protobufBuilderResult.$name$ = [NSMutableArray array];\n"
					"  }\n"
					"  [protobufBuilderResult.$name$ addObject:[NSNumber numberWithInt:_value]];\n"
					"  return self;\n"
					"}\n"
					"- ($classname$_Builder*) addAll$capitalized_name$:(NSArray*) values {\n"
					"  if (protobufBuilderResult.$name$ == nil) {\n"
					"    protobufBuilderResult.$name$ = [NSMutableArray array];\n"
					"  }\n"
					"  [protobufBuilderResult.$name$ addObjectsFromArray:values];\n"
					"  return self;\n"
					"}\n"
					"- ($classname$_Builder*) clear$capitalized_name$List {\n"
					"  protobufBuilderResult.$name$ = nil;\n"
					"  return self;\n"
					"}\n");
}

void RepeatedEnumFieldGenerator::GenerateMergingCodeSource(
		io::Printer* printer) const {
	printer->Print(variables_,
			"if (other.$name$.count > 0) {\n"
					"  if (protobufBuilderResult.$name$ == nil) {\n"
					"    protobufBuilderResult.$name$ = [NSMutableArray array];\n"
					"  }\n"
					"  [protobufBuilderResult.$name$ addObjectsFromArray:other.$name$];\n"
					"}\n");
}

void RepeatedEnumFieldGenerator::GenerateBuildingCodeSource(
		io::Printer* printer) const {
}

void RepeatedEnumFieldGenerator::GenerateParsingCodeSource(
		io::Printer* printer) const {
	// If packed, set up the while loop
	if (descriptor_->options().packed()) {
		printer->Print(variables_, "int32_t length = [input readRawVarint32];\n"
				"int32_t oldLimit = [input pushLimit:length];\n"
				"while (input.bytesUntilLimit > 0) {\n");
		printer->Indent();
	}

	printer->Print(variables_, "int32_t _value = [input readEnum];\n"
			"if ($type$IsValidValue(_value)) {\n"
			"  [self add$capitalized_name$:_value];\n"
			"} else {\n"
			"  [unknownFields mergeVarintField:$number$ value:_value];\n"
			"}\n");

	if (descriptor_->options().packed()) {
		printer->Outdent();
		printer->Print(variables_, "}\n"
				"[input popLimit:oldLimit];\n");
	}
}

void RepeatedEnumFieldGenerator::GenerateSerializationCodeSource(
		io::Printer* printer) const {
	if (descriptor_->options().packed()) {
		printer->Print(variables_, "if (self.$name$.count > 0) {\n"
				"  [output writeRawVarint32:$tag$];\n"
				"  [output writeRawVarint32:$name$MemoizedSerializedSize];\n"
				"}\n"
				"for (NSNumber* element in self.$name$) {\n"
				"  [output writeEnumNoTag:element.intValue];\n"
				"}\n");
	} else {
		printer->Print(variables_, "for (NSNumber* element in self.$name$) {\n"
				"  [output writeEnum:$number$ value:element.intValue];\n"
				"}\n");
	}
}

void RepeatedEnumFieldGenerator::GenerateSerializedSizeCodeSource(
		io::Printer* printer) const {
	printer->Print(variables_, "{\n"
			"  int32_t dataSize = 0;\n");
	printer->Indent();

	printer->Print(variables_, "for (NSNumber* element in self.$name$) {\n"
			"  dataSize += computeEnumSizeNoTag(element.intValue);\n"
			"}\n");

	printer->Print("size += dataSize;\n");

	if (descriptor_->options().packed()) {
		printer->Print(variables_, "if (self.$name$.count > 0) {\n"
				"  size += $tag_size$;\n"
				"  size += computeRawVarint32Size(dataSize);\n"
				"}\n");
	} else {
		printer->Print(variables_, "size += $tag_size$ * self.$name$.count;\n");
	}

	if (descriptor_->options().packed()) {
		printer->Print(variables_,
				"$name$MemoizedSerializedSize = dataSize;\n");
	}

	printer->Outdent();
	printer->Print("}\n");
}

string RepeatedEnumFieldGenerator::GetBoxedType() const {
	return ClassName(descriptor_->enum_type());
}
} // namespace objectivec
} // namespace compiler
} // namespace protobuf
} // namespace google
