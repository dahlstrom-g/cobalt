// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/filters/pipeline_integration_test_base.h"

#include "base/bind.h"
#include "media/base/media_log.h"
#include "media/filters/chunk_demuxer_factory.h"
#include "media/filters/ffmpeg_audio_decoder.h"
#include "media/filters/ffmpeg_demuxer_factory.h"
#include "media/filters/ffmpeg_video_decoder.h"
#include "media/filters/file_data_source.h"
#include "media/filters/null_audio_renderer.h"
#include "media/filters/video_renderer_base.h"

using ::testing::AnyNumber;

namespace media {

PipelineIntegrationTestBase::PipelineIntegrationTestBase()
    : message_loop_factory_(new MessageLoopFactoryImpl()),
      pipeline_(new Pipeline(&message_loop_, new MediaLog())),
      ended_(false),
      pipeline_status_(PIPELINE_OK) {
  EXPECT_CALL(*this, OnVideoRendererPaint()).Times(AnyNumber());
  EXPECT_CALL(*this, OnSetOpaque(true)).Times(AnyNumber());
}

PipelineIntegrationTestBase::~PipelineIntegrationTestBase() {
  if (!pipeline_->IsRunning())
    return;

  Stop();
}

void PipelineIntegrationTestBase::OnStatusCallback(
    PipelineStatus expected_status,
    PipelineStatus status) {
  EXPECT_EQ(status, expected_status);
  pipeline_status_ = status;
  message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
}

PipelineStatusCB PipelineIntegrationTestBase::QuitOnStatusCB(
    PipelineStatus expected_status) {
  return base::Bind(&PipelineIntegrationTestBase::OnStatusCallback,
                    base::Unretained(this),
                    expected_status);
}

void PipelineIntegrationTestBase::OnEnded(PipelineStatus status) {
  DCHECK_EQ(status, PIPELINE_OK);
  DCHECK(!ended_);
  ended_ = true;
  message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
}

bool PipelineIntegrationTestBase::WaitUntilOnEnded() {
  if (ended_)
    return (pipeline_status_ == PIPELINE_OK);
  message_loop_.Run();
  EXPECT_TRUE(ended_);
  return ended_ && (pipeline_status_ == PIPELINE_OK);
}

void PipelineIntegrationTestBase::OnError(PipelineStatus status) {
  DCHECK_NE(status, PIPELINE_OK);
  pipeline_status_ = status;
  message_loop_.PostTask(FROM_HERE, MessageLoop::QuitClosure());
}

bool PipelineIntegrationTestBase::Start(const std::string& url,
                                        PipelineStatus expected_status) {
  pipeline_->Start(
      CreateFilterCollection(url),
      url,
      base::Bind(&PipelineIntegrationTestBase::OnEnded, base::Unretained(this)),
      base::Bind(&PipelineIntegrationTestBase::OnError, base::Unretained(this)),
      NetworkEventCB(),
      QuitOnStatusCB(expected_status));
  message_loop_.Run();
  return (pipeline_status_ == PIPELINE_OK);
}

void PipelineIntegrationTestBase::Play() {
  pipeline_->SetPlaybackRate(1);
}

void PipelineIntegrationTestBase::Pause() {
  pipeline_->SetPlaybackRate(0);
}

bool PipelineIntegrationTestBase::Seek(base::TimeDelta seek_time) {
  ended_ = false;

  pipeline_->Seek(seek_time, QuitOnStatusCB(PIPELINE_OK));
  message_loop_.Run();
  return (pipeline_status_ == PIPELINE_OK);
}

void PipelineIntegrationTestBase::Stop() {
  DCHECK(pipeline_->IsRunning());
  pipeline_->Stop(QuitOnStatusCB(PIPELINE_OK));
  message_loop_.Run();
}

void PipelineIntegrationTestBase::QuitAfterCurrentTimeTask(
    const base::TimeDelta& quit_time) {
  if (pipeline_->GetCurrentTime() >= quit_time ||
      pipeline_status_ != PIPELINE_OK) {
    message_loop_.Quit();
    return;
  }

  message_loop_.PostDelayedTask(
      FROM_HERE,
      base::Bind(&PipelineIntegrationTestBase::QuitAfterCurrentTimeTask,
                 base::Unretained(this), quit_time),
      10);
}

bool PipelineIntegrationTestBase::WaitUntilCurrentTimeIsAfter(
    const base::TimeDelta& wait_time) {
  DCHECK(pipeline_->IsRunning());
  DCHECK_GT(pipeline_->GetPlaybackRate(), 0);
  DCHECK(wait_time <= pipeline_->GetMediaDuration());

  message_loop_.PostDelayedTask(
      FROM_HERE,
      base::Bind(&PipelineIntegrationTestBase::QuitAfterCurrentTimeTask,
                 base::Unretained(this),
                 wait_time),
      10);
  message_loop_.Run();
  return (pipeline_status_ == PIPELINE_OK);
}

scoped_ptr<FilterCollection>
PipelineIntegrationTestBase::CreateFilterCollection(const std::string& url) {
  scoped_refptr<FileDataSource> data_source = new FileDataSource();
  CHECK_EQ(PIPELINE_OK, data_source->Initialize(url));
  return CreateFilterCollection(scoped_ptr<DemuxerFactory>(
      new FFmpegDemuxerFactory(data_source, &message_loop_)));
}

scoped_ptr<FilterCollection>
PipelineIntegrationTestBase::CreateFilterCollection(
    ChunkDemuxerClient* client) {
  return CreateFilterCollection(scoped_ptr<DemuxerFactory>(
      new ChunkDemuxerFactory(client)));
}

scoped_ptr<FilterCollection>
PipelineIntegrationTestBase::CreateFilterCollection(
    scoped_ptr<DemuxerFactory> demuxer_factory) {
  scoped_ptr<FilterCollection> collection(new FilterCollection());
  collection->SetDemuxerFactory(demuxer_factory.Pass());
  collection->AddAudioDecoder(new FFmpegAudioDecoder(
      message_loop_factory_->GetMessageLoop("AudioDecoderThread")));
  collection->AddVideoDecoder(new FFmpegVideoDecoder(
      message_loop_factory_->GetMessageLoop("VideoDecoderThread")));
  collection->AddVideoRenderer(new VideoRendererBase(
      base::Bind(&PipelineIntegrationTestBase::OnVideoRendererPaint,
                 base::Unretained(this)),
      base::Bind(&PipelineIntegrationTestBase::OnSetOpaque,
                 base::Unretained(this))));
  collection->AddAudioRenderer(new NullAudioRenderer());
  return collection.Pass();
}

}  // namespace media
