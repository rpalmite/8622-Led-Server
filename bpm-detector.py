import librosa
import numpy as np
import threading
import time

class BeatDetector:
    def __init__(self, audio_file_path):
        self.audio_file_path = audio_file_path
        self.beat_thread = None
        self.stop_thread = False

    def _detect_beats(self):
        y, sr = librosa.load(self.audio_file_path)

        onset_env = librosa.onset.onset_strength(y, sr=sr)
        tempo, _ = librosa.beat.beat_track(onset_envelope=onset_env, sr=sr)

        beat_frames = librosa.beat.frame_onsets(onset_envelope=onset_env, sr=sr)
        
        for frame in beat_frames:
            if self.stop_thread:
                break
            print(f"Beat detected at time: {librosa.frames_to_time(frame, sr=sr):.2f}s")
            time.sleep(60 / tempo)  # Adjust sleep time based on tempo

    def start_detection(self):
        if self.beat_thread is None or not self.beat_thread.is_alive():
            self.stop_thread = False
            self.beat_thread = threading.Thread(target=self._detect_beats)
            self.beat_thread.start()
            print("Beat detection started.")
        else:
            print("Beat detection is already running.")

    def stop_detection(self):
        if self.beat_thread and self.beat_thread.is_alive():
            self.stop_thread = True
            self.beat_thread.join()
            print("Beat detection stopped.")
        else:
            print("No active beat detection to stop.")

# Example usage
if __name__ == "__main__":
    audio_path = "path/to/your/audio/file.mp3"
    beat_detector = BeatDetector(audio_path)

    # Start beat detection in a separate thread
    beat_detector.start_detection()

    # Allow some time for beat detection (you can perform other tasks here)
    time.sleep(10)

    # Stop beat detection
    beat_detector.stop_detection()