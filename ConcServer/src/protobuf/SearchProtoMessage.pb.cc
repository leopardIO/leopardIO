// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SearchProtoMessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "SearchProtoMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace leopardIO {

namespace {

const ::google::protobuf::Descriptor* SearchProtoMessage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SearchProtoMessage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_SearchProtoMessage_2eproto() {
  protobuf_AddDesc_SearchProtoMessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "SearchProtoMessage.proto");
  GOOGLE_CHECK(file != NULL);
  SearchProtoMessage_descriptor_ = file->message_type(0);
  static const int SearchProtoMessage_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchProtoMessage, content_length_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchProtoMessage, start_time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchProtoMessage, end_time_),
  };
  SearchProtoMessage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SearchProtoMessage_descriptor_,
      SearchProtoMessage::default_instance_,
      SearchProtoMessage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchProtoMessage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SearchProtoMessage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SearchProtoMessage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_SearchProtoMessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SearchProtoMessage_descriptor_, &SearchProtoMessage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_SearchProtoMessage_2eproto() {
  delete SearchProtoMessage::default_instance_;
  delete SearchProtoMessage_reflection_;
}

void protobuf_AddDesc_SearchProtoMessage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\030SearchProtoMessage.proto\022\tleopardIO\"R\n"
    "\022SearchProtoMessage\022\026\n\016content_length\030\001 "
    "\002(\005\022\022\n\nstart_time\030\002 \002(\002\022\020\n\010end_time\030\003 \002("
    "\002", 121);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "SearchProtoMessage.proto", &protobuf_RegisterTypes);
  SearchProtoMessage::default_instance_ = new SearchProtoMessage();
  SearchProtoMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_SearchProtoMessage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_SearchProtoMessage_2eproto {
  StaticDescriptorInitializer_SearchProtoMessage_2eproto() {
    protobuf_AddDesc_SearchProtoMessage_2eproto();
  }
} static_descriptor_initializer_SearchProtoMessage_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int SearchProtoMessage::kContentLengthFieldNumber;
const int SearchProtoMessage::kStartTimeFieldNumber;
const int SearchProtoMessage::kEndTimeFieldNumber;
#endif  // !_MSC_VER

SearchProtoMessage::SearchProtoMessage()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SearchProtoMessage::InitAsDefaultInstance() {
}

SearchProtoMessage::SearchProtoMessage(const SearchProtoMessage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SearchProtoMessage::SharedCtor() {
  _cached_size_ = 0;
  content_length_ = 0;
  start_time_ = 0;
  end_time_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SearchProtoMessage::~SearchProtoMessage() {
  SharedDtor();
}

void SearchProtoMessage::SharedDtor() {
  if (this != default_instance_) {
  }
}

void SearchProtoMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SearchProtoMessage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SearchProtoMessage_descriptor_;
}

const SearchProtoMessage& SearchProtoMessage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_SearchProtoMessage_2eproto();
  return *default_instance_;
}

SearchProtoMessage* SearchProtoMessage::default_instance_ = NULL;

SearchProtoMessage* SearchProtoMessage::New() const {
  return new SearchProtoMessage;
}

void SearchProtoMessage::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    content_length_ = 0;
    start_time_ = 0;
    end_time_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SearchProtoMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 content_length = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &content_length_)));
          set_has_content_length();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(21)) goto parse_start_time;
        break;
      }

      // required float start_time = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_start_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &start_time_)));
          set_has_start_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(29)) goto parse_end_time;
        break;
      }

      // required float end_time = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_end_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &end_time_)));
          set_has_end_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SearchProtoMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 content_length = 1;
  if (has_content_length()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->content_length(), output);
  }

  // required float start_time = 2;
  if (has_start_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->start_time(), output);
  }

  // required float end_time = 3;
  if (has_end_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->end_time(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SearchProtoMessage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 content_length = 1;
  if (has_content_length()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->content_length(), target);
  }

  // required float start_time = 2;
  if (has_start_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->start_time(), target);
  }

  // required float end_time = 3;
  if (has_end_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(3, this->end_time(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SearchProtoMessage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 content_length = 1;
    if (has_content_length()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->content_length());
    }

    // required float start_time = 2;
    if (has_start_time()) {
      total_size += 1 + 4;
    }

    // required float end_time = 3;
    if (has_end_time()) {
      total_size += 1 + 4;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SearchProtoMessage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SearchProtoMessage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SearchProtoMessage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SearchProtoMessage::MergeFrom(const SearchProtoMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_content_length()) {
      set_content_length(from.content_length());
    }
    if (from.has_start_time()) {
      set_start_time(from.start_time());
    }
    if (from.has_end_time()) {
      set_end_time(from.end_time());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SearchProtoMessage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SearchProtoMessage::CopyFrom(const SearchProtoMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SearchProtoMessage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void SearchProtoMessage::Swap(SearchProtoMessage* other) {
  if (other != this) {
    std::swap(content_length_, other->content_length_);
    std::swap(start_time_, other->start_time_);
    std::swap(end_time_, other->end_time_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SearchProtoMessage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SearchProtoMessage_descriptor_;
  metadata.reflection = SearchProtoMessage_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace leopardIO

// @@protoc_insertion_point(global_scope)
